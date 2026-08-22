# Enable kernel configurations for dm-verity and EROFS.

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://dm-verity.cfg"

# Enable kernel configurations for dm-crypt.
SRC_URI:append = " file://dm-crypt.cfg"
