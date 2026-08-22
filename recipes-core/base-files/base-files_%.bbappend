# systemd would create a missing mount point at boot, but the root
# filesystem is read-only, so /data must already exist in the image.
dirs755 += "/data"

# The initramfs has already opened the LUKS2 volume as /dev/mapper/data.
do_install:append() {
    echo "/dev/mapper/data /data ext4 defaults,noatime,nodev,nosuid 0 2" >> ${D}${sysconfdir}/fstab
}

# The fstab entry requests an ext4 filesystem check before /data is mounted.
# systemd skips the check if fsck.ext4 is unavailable, so make it explicit.
RDEPENDS:${PN}:append = " e2fsprogs-e2fsck"
