# This image recipe assigns PACKAGE_INSTALL directly, so extend that variable
# rather than IMAGE_INSTALL. The module's RDEPENDS pulls in the complete unlock
# path, including frdm-luks, cryptsetup, mkfs.ext4 and blkid.
PACKAGE_INSTALL:append = " initramfs-module-dmcrypt"
