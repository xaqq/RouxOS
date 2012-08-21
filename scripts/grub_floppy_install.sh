if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi
dd if=/dev/zero of=$4 bs=512 count=2880
mkfs $4
mkdir -p $1
losetup $2 $4
mount -t ext2 $2 $1
mkdir -p $1/grub/
cp $3/stage[12] $1/grub/
echo "(fd0) /dev/loop0" > $1/grub/device.map
grub --device-map=$1/grub/device.map $2 <<EOF
root (fd0)
setup (fd0)
quit
EOF
umount $1
losetup -d $2