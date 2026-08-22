# base-files already references file://fstab. Prepending this directory makes
# that reference resolve to the copy provided by this layer.
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# The deployed root filesystem is read only, so create the mount point while
# the image is being built.
dirs755 += "/data"

# The fstab entry requests an ext4 filesystem check before /data is mounted.
# systemd skips the check if fsck.ext4 is unavailable, so make it explicit.
RDEPENDS:${PN}:append = " e2fsprogs-e2fsck"
