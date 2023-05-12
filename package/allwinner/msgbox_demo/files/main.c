#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <linux/rpmsg.h>

#define MSGBOX_DEMO_HELP \
	"\nmsgbox_demo [-s MESSAGE] [-r]\n\n" \
	"OPTIONS:\n" \
	" -s MESSAGE : send MESSAGE\n" \
	" -r         : receive messages\n" \
	"e.g.\n" \
	" msgbox_demo -s \"hello\" : send string \"hello\"\n" \
	" msgbox_demo -r         : receive messages\n" \
	"\n"

#define RPMSG_CTRL_DEV "/dev/rpmsg_ctrl0"

#define ENDPOINT_NAME "msgbox_demo"    /* should be less than 32 bytes */
#define ENDPOINT_SRC_ADDR 0x3
#define ENDPOINT_DST_ADDR 0xffffffff

int is_stopped = 0;

static void when_signal(int sig)
{
	printf("signal got\n");
	switch (sig) {
		case SIGINT:
		case SIGQUIT:
		case SIGHUP:
			is_stopped = 1;
			break;
		default:
			break;
	}
}

/**
 * Get the interface "/dev/rpmsgXX" that matches the endpoint name.
 *
 * This function reads all the "/sys/class/rpmsg/rpmsgXX/name" and finds the one
 * that is the same as the endpoint name.
 *
 * @param ept_name: endpoint name (The string length should be less than 32 bytes)
 * @param[out] ept_interface: the interface got (Its size should be large enough
 *                            to store the string "/dev/rpmsgXX")
 * @return: 0 on success, otherwise a negative number
 */
static int get_ept_interface_by_name(const char *ept_name, char *ept_interface)
{
	static const char *class_dir = "/sys/class/rpmsg";
	int ret = 0;
	DIR *dir = NULL;
	struct dirent *dir_entry = NULL;
	char ept_name_buf[32];
	int ept_name_len = strlen(ept_name);
	int is_found = 0;

	dir = opendir(class_dir);
	if (!dir) {
		printf("Failed to open directory \"%s\"\n", class_dir);
		ret = -1;
		goto out;
	}

	while ((dir_entry = readdir(dir))) {
		if (0 != strncmp(dir_entry->d_name, "rpmsg", 5)) {
			continue;
		}
		char name_path[256] = "/sys/class/rpmsg/";
		strcat(name_path, dir_entry->d_name);
		strcat(name_path, "/name");
		FILE *fp = fopen(name_path, "rb");
		if (!fp) {
			continue;
		}
		ret = fread(ept_name_buf, 1, ept_name_len, fp);
		if (ret != ept_name_len) {
			fclose(fp);
			continue;
		}
		ept_name_buf[ept_name_len] = '\0';
		if (0 == strcmp(ept_name, ept_name_buf)) {
			strcpy(ept_interface, "/dev/");
			strcat(ept_interface, dir_entry->d_name);
			is_found = 1;
			fclose(fp);
			break;
		}
		fclose(fp);
	}

	ret = is_found ? 0 : -1;

	closedir(dir);
out:
	return ret;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int c;
	struct rpmsg_endpoint_info ept_info = {
		.name = ENDPOINT_NAME,
		.src = ENDPOINT_SRC_ADDR,
		.dst = ENDPOINT_DST_ADDR,
	};
	int do_send = 0;
	const char *data_send = NULL;
	int do_recv = 0;
	uint32_t data_recv;
	int fd_ctrl;
	int fd_ept;
	char ept_interface[16];
	struct pollfd poll_fds;

	signal(SIGINT, when_signal);
	signal(SIGQUIT, when_signal);
	signal(SIGHUP, when_signal);

	if (argc <= 1) {
		printf(MSGBOX_DEMO_HELP);
		ret = -1;
		goto out;
	}

	while ((c = getopt(argc, argv, "hs:r")) != -1) {
		switch (c) {
		case 'h':
			printf(MSGBOX_DEMO_HELP);
			ret = 0;
			goto out;
		case 's':
			do_send = 1;
			data_send = optarg;
			break;
		case 'r':
			do_recv = 1;
			break;
		default:
			printf(MSGBOX_DEMO_HELP);
			ret = -1;
			goto out;
		}
	}

	fd_ctrl = open(RPMSG_CTRL_DEV, O_RDWR);
	if (fd_ctrl < 0) {
		printf("Failed to open \"%s\" (ret: %d)\n", RPMSG_CTRL_DEV, fd_ctrl);
		ret = -1;
		goto out;
	}

	ret = ioctl(fd_ctrl, RPMSG_CREATE_EPT_IOCTL, &ept_info);
	if (ret < 0) {
		printf("Failed to create endpoint (ret: %d)\n", ret);
		ret = -1;
		goto out;
	}

	ret = get_ept_interface_by_name(ENDPOINT_NAME, ept_interface);
	if (ret < 0) {
		printf("The endpoint interface named \"%s\" not found\n", ENDPOINT_NAME);
		ret = -1;
		goto close_fd_ctrl;
	}

	fd_ept = open(ept_interface, O_RDWR);
	if (fd_ept < 0) {
		printf("Failed to open \"%s\" (ret: %d)\n", ept_interface, fd_ept);
		ret = -1;
		goto close_fd_ctrl;
	}

	if (do_send) {
		write(fd_ept, data_send, strlen(data_send));
	}

	if (do_recv) {
		poll_fds.fd = fd_ept;
		poll_fds.events = POLLIN;
		while (!is_stopped) {
			ret = poll(&poll_fds, 1, -1);
			if (ret < 0) {
				if (errno == EINTR) {
					printf("Signal occurred. Exit\n");
					break;
				} else {
					printf("Poll error (%s)\n", strerror(errno));
					break;
				}
			}
			read(fd_ept, &data_recv, sizeof(uint32_t));
			printf("Received: 0x%x\n", data_recv);
		}
	}

	ioctl(fd_ept, RPMSG_DESTROY_EPT_IOCTL);

	ret = 0;

close_fd_ept:
	close(fd_ept);
close_fd_ctrl:
	close(fd_ctrl);
out:
	return ret;
}
