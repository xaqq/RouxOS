losetup $1 $2
qemu-kvm -m 512 -fda $1
losetup -d $1