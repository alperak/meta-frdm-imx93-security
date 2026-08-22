# base-files already references file://fstab. Prepending this directory makes
# that reference resolve to the copy provided by this layer.
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# The mount point is a directory on the read-only root filesystem, so it
# cannot be created at boot and must already exist in the image.
dirs755 += "/data"

# The fstab entry requests an ext4 filesystem check before /data is mounted.
# systemd skips the check if fsck.ext4 is unavailable, so make it explicit.
RDEPENDS:${PN}:append = " e2fsprogs-e2fsck"
