# Keep dm-verity and dm-crypt in separate fragments
# so either feature can be changed independently.

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://dm-verity.cfg"
SRC_URI:append = " file://dm-crypt.cfg"
