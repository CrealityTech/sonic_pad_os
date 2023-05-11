#!/bin/bash

pwd=$(dirname $0)

# generate nfsmount util script
cat > $1/usr/bin/nfsmount << EOF
#!/bin/sh

NFS_SERVER="192.168.205.200"
MOUNT_POINT="/home"
if [ "\$1" != "" ];then MOUNT_POINT=\$1;fi
mount -t nfs -o nolock,tcp \$NFS_SERVER:/nfs/$USER \$MOUNT_POINT

EOF
chmod 755 $1/usr/bin/nfsmount

exit 0
