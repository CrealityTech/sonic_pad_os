#!/bin/bash


CONFIG_ARCH_TABLE="coco"
CONFIG_CHIP_TABLE="sun50iw6p1 sun50iw8p1 sun8iw12p1 sun8iw15p1 sun8iw16p1"
CONFIG_BOARD_TABLE="evb fpga"
CONFIG_DDR_TABLE="ddr3 ddr4 lpddr3 lpddr4"

# $1: file which wanted to be created
generate_file()
{
	GENERATE_FILE=$1
	rm -rf $GENERATE_FILE

	echo "/*" > $GENERATE_FILE
	echo " *" >> $GENERATE_FILE
	echo " * Automatically generated file; DO NOT EDIT." >> $GENERATE_FILE
	echo " * AllWinner Sunxi Arisc Configuration" >> $GENERATE_FILE
	echo " *" >> $GENERATE_FILE
	echo " */" >> $GENERATE_FILE
}

# select CONFIG_CHIP_TABLE, and only config .config file
get_chip_select()
{
	i=0
	printf "All available chips:\n"
	for CONFIG_CHIP in $CONFIG_CHIP_TABLE ; do
		echo -n "$i. "
		echo ${CONFIG_CHIP}
		i=$(expr $i + 1)
	done
	printf "Choice: "
	read CONFIG_CHIP

	case "$CONFIG_CHIP" in
		0 )
			echo "arch openrisc" >> $CONFIG_FILE
			echo "platform sun50iw6p1" >> $CONFIG_FILE
			ARCH=openrisc
			PLATFORM=sun50iw6p1
		;;
		1 )
			echo "arch d10" >> $CONFIG_FILE
			echo "platform sun50iw8p1" >> $CONFIG_FILE
			ARCH=d10
			PLATFORM=sun50iw8p1
		;;
		2 )
			echo "arch openrisc" >> $CONFIG_FILE
			echo "platform sun8iw12p1" >> $CONFIG_FILE
			ARCH=openrisc
			PLATFORM=sun8iw12p1
		;;
		3 )
			echo "arch openrisc" >> $CONFIG_FILE
			echo "platform sun8iw15p1" >> $CONFIG_FILE
			ARCH=openrisc
			PLATFORM=sun8iw15p1
		;;
		4 )
			echo "arch openrisc" >> $CONFIG_FILE
			echo "platform sun8iw16p1" >> $CONFIG_FILE
			ARCH=openrisc
			PLATFORM=sun8iw16p1
		;;
	esac
}


# select CONFIG_BOARD_TABLE, and only config .config file
get_board_selece()
{
	i=0
	printf "All available boards:\n"
	for CONFIG_BOARD in $CONFIG_BOARD_TABLE ; do
		echo -n "$i. "
		echo ${CONFIG_BOARD}
		i=$(expr $i + 1)
	done
	printf "Choice: "
	read CONFIG_BOARD

	case "$CONFIG_BOARD" in
		[0] )
			echo "board evb" >> $CONFIG_FILE
		;;
		[1] )
			echo "board fpga" >> $CONFIG_FILE
		;;
	esac
}

get_ddr_selece()
{
	i=0
	printf "All available boards:\n"
	for CONFIG_DDR in $CONFIG_DDR_TABLE ; do
		echo -n "$i. "
		echo ${CONFIG_DDR}
		i=$(expr $i + 1)
	done
	printf "Choice: "
	read CONFIG_DDR

	case "$CONFIG_DDR" in
		[0] )
			echo "ddr ddr3" >> $CONFIG_FILE
		;;
		[1] )
			echo "ddr ddr4" >> $CONFIG_FILE
		;;
		[2] )
			echo "ddr lpddr3" >> $CONFIG_FILE
		;;
		[3] )
			echo "ddr lpddr4" >> $CONFIG_FILE
		;;
	esac
}

# only config .config file, and the autoconf.h will be generated
# later by .config.
config_file()
{
	generate_file $CONFIG_FILE
	get_chip_select
	get_board_selece
	get_ddr_selece
}

# generally speaking, we need not do something for CONFIG_FILE exist,
# but we should generated .config for the build system used first time.
auto_config()
{
	if [ -f "$CONFIG_FILE" ]; then
		echo -n ""
	else
		config_file
		CONFIG_FILE_EXIST=1
	fi
}

# for command config
do_config()
{
	if [ $CONFIG_FILE_EXIST -ne 1 ]; then
		config_file
	fi
}

# get .config and generate autoconf.h every run time.
get_config()
{
	auto_config

	grep -q "sun50iw6p1" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			ARCH=openrisc
			PLATFORM=sun50iw6p1
			CPUXARM=arm64
	fi

	grep -q "sun50iw8p1" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			ARCH=d10
			PLATFORM=sun50iw8p1
			CPUXARM=arm64
	fi

	grep -q "sun8iw12p1" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			ARCH=openrisc
			PLATFORM=sun8iw12p1
			CPUXARM=arm32
	fi

	grep -q "sun8iw15p1" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			ARCH=openrisc
			PLATFORM=sun8iw15p1
			CPUXARM=arm32
	fi

	grep -q "sun8iw16p1" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			ARCH=openrisc
			PLATFORM=sun8iw16p1
			CPUXARM=arm32
	fi


	grep -q "fpga" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			BOARD=fpga
	fi

	grep -q "evb" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			BOARD=evb
	fi


	grep -q "ddr3" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			DDR=ddr3
	fi

	grep -q "ddr4" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			DDR=ddr4
	fi

	grep -q "lpddr3" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			DDR=lpddr3
	fi

	grep -q "lpddr4" $CONFIG_FILE
	if [ $? -eq 0 ]; then
			DDR=lpddr4
	fi


	AUTOCONFIG_FILE=./coco/arch/$ARCH/plat-$PLATFORM/bconf.mk
	platform=$(echo $PLATFORM | tr [a-zA-Z] [A-Za-z])
	board=$(echo $BOARD | tr [a-zA-Z] [A-Za-z])
	ddr=$(echo $DDR | tr [a-zA-Z] [A-Za-z])
	arch=$(echo $ARCH | tr [a-zA-Z] [A-Za-z])
	arm=$(echo $CPUXARM | tr [a-zA-Z] [A-Za-z])
	echo CFG_${platform}"  = y" > $AUTOCONFIG_FILE
	echo CFG_${board}_FLATFORM"  = y">> $AUTOCONFIG_FILE
	echo CFG_CPUX_${arm}"  = y">> $AUTOCONFIG_FILE
	echo CFG_${arch}"  = y">> $AUTOCONFIG_FILE
	echo CFG_${ddr}"  = y">> $AUTOCONFIG_FILE
	echo DRAM_TYPE_${ddr}"  = y">> $AUTOCONFIG_FILE
}

# $1: the string to echo
echo_suc()
{
	echo -e "\033[34;49;1m $1 \033[39;49;0m"
}

# $1: the string to echo
echo_err()
{
	echo -e "\033[31;49;1m $1 \033[39;49;0m"
}

show_help()
{
	printf "niubi"
}


get_version()
{
	ver=$(git rev-parse  HEAD)
	echo "arisc version: $ver"
}

# setup compile environments: toolchain and libary path
export_tools_envir()
{
	if [ $ARCH = "openrisc" ]; then
		#check should uncompress toolchain package
		if [ ! -d ./coco/tools/openrisc_toolchain ]; then
			if [ -d tools ]; then
				echo uncompress toolchain package...
				tar  -jxvf coco/tools/openrisc_toolchain.tar.bz2 -C tools/arch_toolchain
				echo uncompress toolchain finish
			fi
		fi
		rm -rf $PWD/coco/tools/toolchain
		cp -rf $PWD/coco/tools/arch_toolchain/openrisc_toolchain $PWD/coco/tools/toolchain
		LIBPATH=$PWD/coco/tools/openrisc_toolchain/lib/gcc/or32-elf/4.5.1-or32-1.0rc4
	elif [ $ARCH = "d10" ]; then
		echo "d10"
		#check should uncompress toolchain package
		if [ ! -d ./coco/tools/d10_toolchain ]; then
			if [ -d tools ]; then
				echo uncompress toolchain package...
				tar  -jxvf coco/tools/d10_toolchain.tar.bz2 -C tools/arch_toolchain
				echo uncompress toolchain finish
			fi
		fi
		rm -rf $PWD/coco/tools/toolchain
		cp -rf $PWD/coco/tools/arch_toolchain/d10_toolchain $PWD/coco/tools/toolchain
		LIBPATH=$PWD/coco/tools/d10_toolchain/lib/gcc/nds32le-elf/4.9.3
	fi

	export PATH=$PATH:$PWD/coco/tools/toolchain/bin:$PWD/coco/tools/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/coco/tools/toolchain/lib
}

ARCH=""
OPTION=$1
VERSION=""
CONFIG_FILE_EXIST=0
CONFIG_FILE=./coco/.config

#get config information evertime
get_config

# export tools enviroments
export_tools_envir

# setup compile entironments: toolchain and libary path
VER_FILE=./coco/system/init.c


case "$1" in
clean)
	make clean ARCH=$ARCH PLATFORM=$PLATFORM
	;;
config)
	#config platform
	do_config
	;;
make)
	#get version
	get_version
#	make -C ./coco/dram_code ARCH=$ARCH PLATFORM=$PLATFORM VER=$ver
	make ARCH=$ARCH PLATFORM=$PLATFORM VER=$ver
	touch $VER_FILE
	;;
help)
	show_help
	;;
*)
	if [ $# == 0 ]; then
		get_version
		# make $ARCH system image
#		make -C ./coco/dram_code ARCH=$ARCH PLATFORM=$PLATFORM VER=$ver
		make ARCH=$ARCH PLATFORM=$PLATFORM VER=$ver
	else
		show_help
	fi
	;;
esac
