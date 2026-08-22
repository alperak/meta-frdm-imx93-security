# secure-boot.cfg enables FIT and AHAB.
# The patch adds secure_bootcmd to the board environment.

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
    file://secure-boot.cfg \
    file://0001-imx93_frdm-add-secure_bootcmd-for-FIT-boot.patch \
"
