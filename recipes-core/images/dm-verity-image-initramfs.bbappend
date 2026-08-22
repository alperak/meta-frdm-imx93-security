# This image recipe assigns PACKAGE_INSTALL directly, so extend that variable
# rather than IMAGE_INSTALL. The module's RDEPENDS pull in the rest of the
# unlock path.
PACKAGE_INSTALL:append = " initramfs-module-dmcrypt"
