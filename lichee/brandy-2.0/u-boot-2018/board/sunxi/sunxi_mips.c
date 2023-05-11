#include <common.h>
#include <asm/io.h>
#include <smc.h>
#include <fdt_support.h>

#define RESERVER_MEM_FOR_MIPS ((44 + 30) * 1024 * 1024)
extern int do_sunxi_flash(cmd_tbl_t *cmdtp, int flag, int argc,
			  char *const argv[]);

int do_boot_mips(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret		  = 0;
	uint32_t img_addr = 0;
	char *read_argv[5];

#if defined(FPGA_PLATFORM)
	/*init uart gpio*/
	writel(0x00000200, SUNXI_PIO_BASE + 0x34);
#endif
	read_argv[0] = "sunxi_flash";
	read_argv[1] = "read";
	read_argv[2] = argv[1];
	read_argv[3] = "mips";
	read_argv[4] = NULL;

	ret = do_sunxi_flash(0, 0, 4, read_argv);
	if (ret) {
		if (ret == -ENODEV) {
			pr_err("mips not found, skip loading mips");
			return 0;
		}
		pr_err("%s read mips failed with:%s\n", ret);
		return ret;
	}
	img_addr = simple_strtoul(argv[1], NULL, 16);
	smc_tee_setup_mips(img_addr, 4 * 1024 * 1024);
	ret = fdt_add_mem_rsv(working_fdt, img_addr, RESERVER_MEM_FOR_MIPS);
	if (ret) {
		pr_err("##add mem rsv error: %s : %s\n", __func__,
		       fdt_strerror(ret));
		return -1;
	}
	return 0;
}

static char boot_mips_help[] =
	"[addr] - boot mips application loaded at addr\n";

U_BOOT_CMD(boot_mips, CONFIG_SYS_MAXARGS, 1, do_boot_mips,
	   "boot application image from memory", boot_mips_help);
