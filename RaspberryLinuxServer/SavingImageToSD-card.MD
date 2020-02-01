ADdress

/dev/mmcblk0

Name:
ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz

sudo dd if=~/Downloads/ubuntu-18.04.3-server-arm64.iso of=/dev/mmcblk0 bs=32M
sync

IF ".XZ"
xzcat ~/Downloads/ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz | sudo dd of=/dev/mmcblk0 bs=32M
sync

