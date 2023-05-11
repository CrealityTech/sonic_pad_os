
###Tina Linux快速入门指南 V2.0
  *		适用范围：Tina Linux SDK V2.0版本
  *		更新日期：2017年3月27日
***
[TOC]

####***一. 开发流程***
  1. 构建linux系统编译环境（软件和硬件）
  2. 下载R40 Tina Linux软件包
  3. 对下载的软件包进行配置、编译和打包
  4. 使用LiveSuit烧写固件，minicom进行实时观察

***具体过程***
  1. 构建linux系统编译环境
  (1) 硬件资源
 	 * R40系列开发板一块
 	 * 电源线、usb转串口线、micro-usb下载线
 	 * pc和编译服务器

  (2) 软件资源
 	 * 编译使用Ubuntu14.04系统
 	 * 需要安装的软件包如下：gcc, binutils, bzip2, flex, python,perl, make,ia32-libs, find, grep, diff, unzip, gawk,getopt, subversion, libz-dev and libc headers.ubuntu,可使用如下的命令直接安装相关软件包：sudo apt-get install build-essential subversion git-core libncurses5-dev zlib1g-dev gawk flex quilt libssl-dev xsltproc libxml-parser-perl mercurial bzr ecj cvs unzip ia32-libs
 	 *	LiveSuit和minicom，LiveSuit用于把固件烧写到开发板,通过minicom进行烧写过程中观察和后续使用（LiveSuit见http://www.everything.pld1.local/C%3A/users/pld/%E6%A1%8C%E9%9D%A2/pld-know/3_%E5%B8%B8%E7%94%A8%E8%BD%AF%E4%BB%B6/7_%E8%87%AA%E5%8A%A8%E5%AE%89%E8%A3%85/auto_install_for_x64/LiveSuit )

  2. 下载R40 Tina Linux软件包
    *  下载repo并将其放到/usr/bin目录下（见Repo_Guide.md)
	*  进入home目录，执行以下指令
	* $ mkdir -p workspace/tina
    * $ cd workspace/tina
    * $ repo init -u http://gerrit.allwinnertech.com:8081/product/tina/manifest.git -b tina-dev -m default.xml
	* $ repo sync
	* $ repo start tina-dev --all

  3. 对下载的软件包进行配置、编译和打包
	* 进入到/home/workspace/tina目录，执行以下指令
	* $ source build/envsetup.sh
	* $ lunch r40_m2ultra-tina
	* $ make -j2
	* $ pack -d
	* 编译完成后系统镜像会自动打包在tina/out/< board >/目录下

  4. 使用LiveSuit烧写固件，minicom进行实时观察
	* 将开发板和PC进行正确连接
	* 启动LiveSuit选择需要下载的固件
	* 启动minicom进行配置（端口：ttyUSB0、波特率：115200 8N1、连接类型：serial、	Hardware Flow Control 设 为 NO)
	* 按下开发板上uboot按键不要松开，再按一下reset按键，PC机上会提示：是否格式化数据分区？选择Yes或No都可以。然后其自动烧写镜像到开发板，在minicom上可以看到具体过程，等待烧写完成

####***二. Tina Linux Sdk目录结构***

Tina Linux SDK主要由构建系统、配置工具、工具链、host工具包、目标机功能包、文档、脚本、linux内核、bootloader组成。
[点击查看Tina结构图](https://www.processon.com/view/link/58d86685e4b03e064c7b8c34)
Tina/
├── build
├── config
├── Config.in
├── dl
├── docs
├── lichee
├── Makefile
├── out
├── package
├── rules.mk
├── scripts
├── target
├── tmp
├── toolchain
└── tools

- build 目录
主要由一系列.mk文件组成，它们是基于Makefile规格编写而成。主要功能有：
    - 检测当前的编译环境是否满足Tina linux的构建需求
    - 生成host包编译规格
    - 生成工具链的编译规则
    - 生成target包的编译规则
    - 生成linux kernel的编译规则
    - 生成系统固件的生成规格
- config 目录
config/
├── Config-build.in
├── Config-devel.in
├── Config-images.in
├── Config-kernel.in
└── top_config.in
这里存放的都是用于菜单配置项的.in文件，当执行make menuconfig时发挥作用。这里简单介绍所有调用到的.in文件，仅供参考：
[点击查看.in目录结构](https://www.processon.com/view/link/58d86b46e4b04583185077cb)
    - top_config.in: 可视化配置主界面，调用其他config目录下的.in文件，初始化配置选项，用到了source命令
    - Config-xxx.in: 各种可视化界面中的编译选项，含default值
    - tmp/.config-package.in: 各种包支持，包含固件生成，文件系统，各种服务等
    - target/Config.in: 调用tmp/.config-target.in并配置硬件特性
    - toolchain/Config.in: 优化相关选项，target option，update toolchain
    - tmp/.config-target.in: 开发板相关选项，系统相关选项
    - target/imagebuilder/Config.in: 建立OpenWRT Image，包含package文件
    - target/sdk/Config.in: 建立OpenWRT SDK
    - target/toolchain/Config.in: 打包OpenWRT 基础工具链
- docs 目录
docs/
├── build.md
├── config.md
├── init-scripts.md
├── Makefile
├── network.md
├── tina.md
├── wireless.md
└── working.md
存放用于开发的文档，以markdown格式编写。后续不断完善。
- lichee 目录
lichee/
├── brandy
│   ├── add_hash
│   ├── add_hash.sh
│   ├── arm-trusted-firmware-1.0
│   ├── armv8_toolchain
│   ├── basic_loader
│   ├── build.sh
│   ├── extern-libs
│   ├── gcc-linaro
│   ├── pack_tools
│   ├── toolchain
│   ├── u-boot-2011.09
│   └── u-boot-2014.07
├── linux-3.10
└── linux-3.4
其中brandy目录用于存放bootloader,其中u-boot-2011.09用于存放R8/R16/R58的bootloader，u-boot-2014.07用于存放R18/R40的bootloader；linux-3.10是R18/R40使用的内核源码目录；linux-3.4是R8/R16/R58使用的内核源码目录。
- package 目录
package/
├── base-files
├── devel
├── firmware
├── kernel
├── libs
├── Makefile
├── network
├── system
└── utils
package 目录用于存放target机器上的软件包的源码和编译规格，目录按照目标软件包的功能进行分类。
- scripts 目录
scripts 目录用于存放pc端或者小机端使用的一些脚本
- target 目录
target/
├── allwinner
├── Config.in
├── imagebuilder
├── Makefile
├── sdk
└── toolchain
用于存放目标板相关的配置以及sdk和toolchain生成的规格
- toolchain目录
toolchain/
├── binutils
├── Config.in
├── fortify-headers
├── gcc
├── gdb
├── glibc
├── info.mk
├── insight
├── kernel-headers
├── Makefile
├── musl
└── wrapper
其中gcc目录为gcc编译器的编译规则目录，gdb为c／c++调试器编译规则目录，glibc目前没有启用，默认使用的是musl下的musl-libc
- tools 目录
tools目录下存放的是host端工具的编译规则
- Makefile 结构
最后整理一下Makefile的结构，仅供参考：
[Tina Makefile](https://www.processon.com/view/link/58d88239e4b03eea78334943)

####***三. 编译系统的内部工作原理之构建系统工作流程***
![](http://p1.bqimg.com/567571/743537a96715fa38.png)

####***四. Tina系统的 makefile简要分析***
  * ***Tina - Makefile结构图***
![](https://www.processon.com/chart_image/58d8aebee4b045831853d0c0.png)

**顶层Makefile：** 里面只include了build/main.mk文件
**Build/main.mk：**
  - 为Openwrt检查tina目录，此目录不能包含空格。
  - Include了build/host.mk
  - 检查Openwrt是否安装
  - 包含rules.mk build/depends.mk target/Makefile package/Makefile tools/Makefile等Makefile文件

**Build/host.mk：**识别主机所使用工具链选项，主机的系统，主机构架，主机名字等，并在临时目录tmp下生成tmp/.host.mk 。tmp/host.mk里如下：
	- HOST_OS:=Linux
	- HOST_ARCH:=x86_64
	- GNU_HOST_NAME:=x86_64-linux-gnu
	- FIND_L=find -L $(1)

**rules.mk : ** 里面定义了主机和目标机中软件工具包的配置规则
**build/depends.mk : **里面定义了子树上的依赖关系
**build/subdir.mk : **主要包含了subdir和stampfile函数
**target/Makefile ： ** 定义生成目标机软件工具包的生成规格
**package/Makefile : ** 利用源码生成软件包
**tools/Makefile : ** 定义生成主机端工具编译规则，编译时获取和编译这些工具
**toolchain/Makefile : ** 利用工具链源码生成工具链

####***五. adb简介和使用***

#####5.1 adb简介
  * adb全称Android Debug Bridge，对于PC和手机.平板.开发板之间，起到调试桥的作用。借助adb工具， 我们可以对设备进行管理，还可以进行很多操作，比如运行shell命令,向开发板发送文件等。

#####5.2 adb的安装和配置
  * adb的安装
  	* $sudo apt-get install android-tools-adb
  * adb的配置
	1. 将开发板和电脑正确链接，启动minicom或putty，要在minicom终端里看见开发板里系统正常启动并进入shell，如果没有烧写Tina系统到开发板，请先烧写系统
	2. 打开电脑上的终端，运行命令lsusb，查看设备，找到电脑和开发板相连接的串口，如下：
![](http://i1.piimg.com/567571/360cae93370f3f4f.png)
具体看自己的电脑，也可以先拔下插在电脑上的串口USB，在电脑上使用lsusb命令查看设备；然后再插上串口，再使用lsusb命令查看设备，对比前后两次命令列出列表的不同，定位到设备
	3. 使用命令 sudo vim /etc/udev/rules.d/50-android.rules 打开电脑上的50-android.rules文件（如果没有就创建一个）进行配置，这样电脑就能识别到此串口。具体配置如下(即在此文件里写入)：
  ![](http://i4.buimg.com/567571/2902b8a93c751d3c.png)
  SUBSYSTEM选项不变,ATTR{idVendor}选项中填写设备ID（设备ID见上图，根据自己设备填写）,ATTR{idProduct}填写设备产品ID（设备产品ID见上图），MODE选项不变。保存退出。然后运行以下命令：
 	  - sudo chmod a+rx /etc/udev/rules.d/50-android.rules
 	  - sudo /etc/init.d/udev restart
	4. $sudo vim ~/.android/adb_usb.ini,打开adb_usb.ini文件，向里面写入0x067b(设备ID)
	5.  打开minicom，在shell里输入adbd命令，确保开发板里的adb服务正常（若出现错误，则不能使用）。然后在电脑端的运行以下命令：
  	  - sudo adb kill-server
  	  - sudo adb start-server
  	  - adb devices
  运行完成之后即可看见设备清单，如下：![](http://i4.buimg.com/567571/753ec38177efd46d.png)

#####5.3 adb的使用
  * adb的主要命令如下：
  	#####**adb devices**
 	 - 说明：查看当前连接的设备， 连接到计算机的设备或者模拟器将会列出显示
 	 - adb start-server
 	 - 说明：启动adb

	#####adb kill-server
  	 - 说明：关闭adb

    #####adb shell
	  - 说明：这个命令将登录设备的shell, 后面加直接运行设备命令， 相当于执行远程命令

    #####adb push
 	  - 说明：此命令可以把电脑上的文件复制到开发板上
 	  - 举例： adb push hello.c /tmp/ 这样就把电脑上的hello.c文件复制到了开发板上的/tmp目录下

	#####adb pull
	  -	说明：此命令可以把开发板上的文件复制到电脑上
	  - 举例： adb pull /tmp/hello.c ./ 将开发板上/tmp目录下的文件hello.c复制到当前目录

    #####adb version
	  - 说明：查看adb的版本

	#####adb help
	  - 说明：adb的帮助，有关更多命令详情，请使用adb help命令查看


####***六. 编写程序在开发板上运行***

  #####6.1 准备:
  - 首先将开发软件的工具链的所在目录添加进环境变量里。对于Tina系统来说，其arm工具链在tina/prebuilt/gcc/linux-x86/arm/toolchain-sunxi/toolchain/bin目录下。然后在自己的用户目录下打开.bashrc文件，在这个文件的最后面添加一句话，如下：
  export PATH=$PATH:/home/xxxxx/tina/prebuilt/gcc/linux-x86/arm/toolchain-sunxi/toolchain/bin
  保存后退出，然后注销系统，重新登录。登录之后在终端里输入命令env查看一下PATH变量里是否包含此目录，若包含则添加成功。

#####6.2 应用程序：
  1. 编写一个简单的hello world程序
  2. 用交叉工具链里的arm-openwrt-linux-gcc对其进行编译
  3. 通过adb下载到开发板上，然后运行

#####6.3 驱动程序：
  ######6.3.1 动态加载驱动程序
  1. 编写一个驱动模块，其内容如下：
  ![](http://i2.buimg.com/567571/7caa6a0a55430e99.png)
  2. 编写相应的Makefile，其内容如下：
  ![](http://i1.piimg.com/567571/75aba60817c3372d.png)
  3. 执行make,将会产生hello.ko文件
  4. 将hello.ko文件通过adb下载到开发板上
  5. 执行以下命令进行加载,查看和卸载
- $insmod hello.ko
- $lsmod
- rmmod hello


######6.3.2 静态加载驱动程序
  1. 在tina/lichee/linux-3.10/drivers/char/目录下创建一个hello目录
  2. cd hello，然后编写一个驱动模块，其内容和上面一样。
  3. 创建一个Makefile文件，向里面写入如下内容，保存退出
  ![](http://i4.buimg.com/567571/9f61625e7784171e.png)
  4. 创建一个Kconfig文件，向里面写入如下内容保存退出
  ![](http://i1.piimg.com/567571/430ffd73363e73c1.png)
  5. cd ../，进入到char目录里，打开Makefile，向里面添加如下内容保存退出
![](http://i4.buimg.com/567571/3b959db9bc4dddd4.png)
  6. 同样在char目录下，打开Kconfig文件，向里面添加如下内容
  ![](http://i2.buimg.com/567571/6d4d758d19dfa779.png)
  7. 进入到tina/target/allwinner/< board >/目录下，打开modules.mk文件，向里面写入如下内容保存退出
  ![](http://i4.buimg.com/567571/3e063d821a10d8f2.png)
  8. 在同样的目录下，打开deconfig文件，向里面写入如下内容保存退出
  ![](http://i2.buimg.com/567571/2e648708f61db358.png)
  9. 然后回退到linux-3.10目录下，输入命令make menuconfig，选中Device Drivers--->Character devices--->hello test driver，确保其选项为M，若不是M，则按空格键选择为M，保存退出
  10. 回到tina目录下，重新编译打包
  11. 用LiveSuit下载打包后的镜像到开发板，通过minicom进入到开发板的终端
  12. 使用命令lsmod查看是否有hello模块，若没有则执行insmod hello.ko命令安装驱动

