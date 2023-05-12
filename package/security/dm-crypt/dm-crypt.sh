#!/bin/sh

show_help()
{
	echo "dm-crypt.sh - this script helps you to use the secret partition."
	echo "Usage: $1 <op_flag> <type> <keyfile>"
	echo -e "\t <op_flag>: \n\t\t'c' - create & format secret partition; \n\t\t'm' - mount secret partition; \n\t\t'u' - unmount secret partition and close mapper device"
	echo -e "\t <type>: Device type, can be 'plain' or 'luks'."
	echo -e "\t <keyfile>: Key file, can be 'keyfile' or 'pass' or 'optee-pass'"
}

###################### prepare secret partition  ##############################
# Function: format the secret partition
# param1:   mode type, "plain" or "luks"
# param2:   key type, "keyfile" or "passphrase"
###############################################################################
prepare_sec_part()
{
	# create mapped device
	if [ "x$1" == "xplain" ]; then
		if [ "x$2" == "xkeyfile" ]; then
			# don't save src key file, just encrypt the src key file
			dd if=/dev/urandom bs=1024 count=4 | \
				openssl aes-256-cbc -pass pass:"${passphrase1}" -out ${ENC_KEY_FILE}

			# use src key file to create mapped device
			openssl aes-256-cbc -pass pass:"${passphrase1}" -d -in ${ENC_KEY_FILE} | \
				cryptsetup create --key-file=- --type=plain ${MAP_NAME} ${SRC_DEV}
		else
			echo ${passphrase1} | cryptsetup create --type=plain ${MAP_NAME} ${SRC_DEV} > /dev/null
		fi
	else
		if [ "x$2" == "xkeyfile" ]; then
			# don't save src key file, just encrypt the src key file
			dd if=/dev/urandom bs=1024 count=4 | openssl aes-256-cbc -pass pass:"${passphrase1}" -out ${ENC_KEY_FILE}

			#cryptsetup -v --key-file=- -c aes-xts-plain -s 512 -h sha512 luksFormat ${SRC_DEV}
			openssl aes-256-cbc -pass pass:"${passphrase1}" -d -in ${ENC_KEY_FILE} | \
				cryptsetup --key-file=- -c aes-xts-plain64 -s 256 -h sha256 luksFormat ${SRC_DEV} > /dev/null

			# open LUKS
			openssl aes-256-cbc -pass pass:"${passphrase1}" -d -in ${ENC_KEY_FILE} | \
				cryptsetup --key-file=- -c aes-xts-plain64 -s 256 -h sha256 luksOpen ${SRC_DEV} ${MAP_NAME} > /dev/null
		else
			echo ${passphrase1} | cryptsetup -c aes-xts-plain64 -s 256 -h sha256 luksFormat ${SRC_DEV} > /dev/null
			echo ${passphrase1} | cryptsetup -c aes-xts-plain64 -s 256 -h sha256 luksOpen ${SRC_DEV} ${MAP_NAME} > /dev/null
		fi
	fi

	if [ $? -ne 0 ]; then
		echo [$0]: "failed to create /dev/mapper/secret"
		return
	fi

	echo -e "\nCreating Filesystems...\n$HR"
	# format the mapped secret
	mkfs.ext4 ${DST_DEV} > /dev/null

	# close mapped device
	cryptsetup close ${MAP_NAME}
}

mount_sec_part()
{
	# check the existence of /mnt/secret
	mkdir -p ${MNT_POINT}
	if [ ! -d ${MNT_POINT} ]; then
		echo "No such directory: ${MNT_POINT}"
		exit
	fi

	# open secret
	if [ "x$1" == "xplain" ]; then
		if [ "x$2" == "xkeyfile" ]; then
			openssl aes-256-cbc -pass pass:"${passphrase1}" -d -in ${ENC_KEY_FILE} | \
				cryptsetup open --key-file=- --type=plain ${SRC_DEV} ${MAP_NAME}
		else
			# open secret
			echo ${passphrase1} | cryptsetup open ${SRC_DEV} --type=plain ${MAP_NAME}
		fi
	else
		if [ "x$2" == "xkeyfile" ]; then
			# open LUKS
			openssl aes-256-cbc -pass pass:"${passphrase1}" -d -in ${ENC_KEY_FILE} | \
				cryptsetup --key-file=- -c aes-xts-plain64 -s 256 -h sha256 luksOpen ${SRC_DEV} ${MAP_NAME} > /dev/null
		else
			echo ${passphrase1} | cryptsetup -c aes-xts-plain64 -s 256 -h sha256 luksOpen ${SRC_DEV} ${MAP_NAME} > /dev/null
		fi
	fi

	# fsck secret
	/usr/sbin/fsck.ext4 -y ${DST_DEV} &>/dev/null

	mount -t ext4 ${DST_DEV} ${MNT_POINT} 2>/dev/null
	if [ "$?" -ne "0" ]; then
		cryptsetup close ${MAP_NAME}
		echo "ERROR: wrong key!"
		exit
	else
		echo "mount ${DST_DEV} to ${MNT_POINT}"
	fi
}

###############################################################################
if [ $# -ne 3 ]; then
	show_help $0
	exit
fi

if [ "x$1" != "xc" -a "x$1" != "xm" -a "x$1" != "xu" ]; then
	echo "ERR: <op_flag> should be 'c' or 'm' or 'u'"
	show_help $0
	exit
fi

if [ "x$2" != "xplain" -a "x$2" != "xluks" ]; then
	echo "ERR: <type> should be 'plain' or 'luks'"
	show_help $0
	exit
fi

if [ "x$3" != "xkeyfile" -a "x$3" != "xpass"  -a "x$3" != "xoptee-pass" ]; then
	echo "ERR: <keyfile> should be 'keyfile' or 'pass' or 'optee-pass'"
	show_help $0
	exit
fi

# define src device and mapped device
SECRET_PART=` sed 's/ /\n/g' /proc/cmdline | awk -F'=' '/^partition/{print $2}' | sed 's/:/\n/g'|awk -F'@' '/^secret/{print $2}' `

echo ${SECRET_PART} | grep ubi > /dev/null

if [ $? -ne 0 ]; then
	SRC_DEV=/dev/by-name/secret
else
	SRC_DEV=/dev/by-name/secret-mtd
	SECRET_PART=` cat /proc/mtd  | grep secret | awk -F : '{print $1}' | sed 's/mtd/mtdblock/g' `
	rm -rf /dev/by-name/secret-mtd
	ln -s /dev/${SECRET_PART} /dev/by-name/secret-mtd
fi

ROOT_DEV=/dev/${SECRET_PART}
MAP_NAME=secret
DST_DEV=/dev/mapper/${MAP_NAME}
###############################################################################
# If there is no partition for dm-crypt, there is another way:
# $ dd if=/dev/zero of=/mnt/UDISK/secret.img bs=1M count=2
# $ losetup /dev/loop0 /mnt/UDISC/secret.img
# SRC_DEV=/dev/loop0
###############################################################################

MNT_POINT=/mnt/secret

# for security, we don't save src key file as a file.
# SRC_KEY_FILE=/tmp/source-key-file
ENC_KEY_FILE=encrypt-key-file

if [ ! -e ${SRC_DEV} ]; then
	echo "${SRC_DEV} is not exit. Exit."
	exit
fi

# umount and close secret, just in case
mount | grep "${DST_DEV}" > /dev/null && umount ${DST_DEV}
[ -e ${DST_DEV} ] && cryptsetup close ${MAP_NAME}

if [ "x$1" == "xu" ]; then
	exit
fi

if [ "x$3" != "xoptee-pass" ]; then
	# enter passphrase
	read -rsp "Enter passphrase:" passphrase1
	echo ""
	read -rsp "Enter same passphrase again:" passphrase2

	if [ "x${passphrase1}" != "x${passphrase2}" ]; then
		echo "Error: Passphrases do not match. Exit!"
		exit 1
	fi
else
	if [ -f /usr/bin/getdmkey_na ]; then
		passphrase1=`/usr/bin/getdmkey_na`
		if [ $? -ne 0 ]; then
			echo "Error: can not get dm-crypt key"
			exit 1
		fi
	else
		echo "Error: no optee ta"
		exit 1
	fi
fi


if [ "x$1" == "xc" ]; then
	prepare_sec_part $2 $3
else
	mount_sec_part $2 $3
fi
