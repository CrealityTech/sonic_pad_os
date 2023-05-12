#!/bin/sh
set -e
set -x

SED="${SED:-sed -e}"

ver="$1"
dst_dir="$2"

usr_bin_dir="$dst_dir/usr/bin"

if [ -d "$usr_bin_dir" ] ; then
	$SED "1"'!'"b;s,^#"'!'".*python.*,#"'!'"/usr/bin/python${ver}," -i --follow-symlinks $usr_bin_dir/*
fi
