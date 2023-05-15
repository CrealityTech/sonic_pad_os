#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
	// create socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == listenfd) {
		printf("create socket error");
		return -1;
	}
	
	// bind port 
	struct sockaddr_in bindaddr;
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindaddr.sin_port = htons(3000);
	if(-1 == bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr))) {
		printf("bind error");
		return -1;
	}
	
	// start listen
	if (listen(listenfd, 2) == -1) {
		printf("listem error");
		return -1;
	}
	
	while (1) {
		struct sockaddr_in clientaddr;
		socklen_t clientaddrlen = sizeof(clientaddr);
		
		// accept connection
		int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
		if (clientfd != -1) {         	
			char recvBuf[32] = {0};
			
			// receive data
			int ret = recv(clientfd, recvBuf, 32, 0);
			if (ret > 0) {
				printf("recv data from client, data: %s,ret=%d\n", recvBuf,ret);
				if(strstr(recvBuf,"recovery")) {
					ret = send(clientfd, "ok", 2, 0);
					system("/usr/share/script/recovery.sh part");
				} else {
					ret = send(clientfd, recvBuf, strlen(recvBuf), 0);
				}
				if (ret <= 0)
					printf("send data error.");
			} else {
				printf("recv data error.");
			}
			
			close(clientfd);
		}
	}
	
	//close socket
	close(listenfd);
	return 0;
}
