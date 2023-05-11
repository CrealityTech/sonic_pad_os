r6-mic2 方案对应的开发板的 flash 有两种：SPI NOR 和 SPI NAND，
不同的 flash 的需要选择不同配置的固件进行烧录

r6-mic2 默认选用 NOR 的配置，若想编译 NAND 的固件，可选择以下两种方法中的一种
进行操作：

################################################################################
方法一： 使用预先准备好的 NAND 配置文件

在本目录（files_for_nand）中有 NAND 配置的相关文件，替换掉其同名文件即可
（注意：替换掉同名文件后需要重新执行 make menuconfig 等配置操作，确保自己需要的
选项配置正确）

假设当前为 Tina SDK 的根目录，替换相关配置文件的操作为：
    cp ./target/allwinner/r6-mic2/files_for_nand/config-3.10 ./target/allwinner/r6-mic2/
    cp ./target/allwinner/r6-mic2/files_for_nand/defconfig ./target/allwinner/r6-mic2/
    cp ./target/allwinner/r6-mic2/files_for_nand/sys_config.fex ./target/allwinner/r6-mic2/configs/

################################################################################
方法二： 手动修改配置

1. 执行 make kernel_menuconfig
    1) [*] Enable the block layer --> 选择 [*] Support for large (2TB+) block devices and files
    2) Device Drivers --> [*] Block devices --> 选择 <*> sunxi nand flash driver
    3) Device Drivers --> 取消选择 < > Memory Technology Device (MTD) support
    4) File systems --> 选择 <*> The Extended 4 (ext4) filesystem
    5) File systems --> [*] Miscellaneous filesystems -->
            取消选择 < > Journalling Flash File System v2 (JFFS2) support (若没有可忽略)

2. 执行 make menuconfig
    1) Utilities --> 取消选择 < > mtd-utils
    2) Utilities --> Filesystem --> 选择 <*> e2fsprogs

3. 修改 sys_config.fex (位于 target/allwinner/r6-mic2/configs 目录下)
    将 "storage_type = 3" 改为 "storage_type = 5"

################################################################################

进行上述修改后，重新编译并打包固件即可
