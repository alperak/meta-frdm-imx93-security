# Add the module that opens the encrypted data partition.
#
# Keep the filename to one hyphen. initramfs-framework derives the function
# prefix with `basename $m | cut -d'-' -f 2`, so 81-dm-crypt would incorrectly
# look for dm_enabled() and dm_run().
FILESEXTRAPATHS:prepend := "${THISDIR}/initramfs-framework-dmcrypt:"

SRC_URI:append = " file://dmcrypt"

do_install:append() {
    install -m 0755 ${S}/dmcrypt ${D}/init.d/81-dmcrypt
}

PACKAGES:append = " initramfs-module-dmcrypt"

SUMMARY:initramfs-module-dmcrypt = "initramfs dm-crypt encrypted data partition support"

FILES:initramfs-module-dmcrypt = "/init.d/81-dmcrypt"

RDEPENDS:initramfs-module-dmcrypt = " \
    ${PN}-base \
    cryptsetup \
    frdm-luks \
    e2fsprogs-mke2fs \
    util-linux-blkid \
"
