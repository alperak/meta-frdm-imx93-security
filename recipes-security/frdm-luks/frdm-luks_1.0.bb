SUMMARY = "Trusted Application and client for the data partition unlock credential"
DESCRIPTION = "Derives an encrypted volume's unlock secret from the i.MX93 \
OP-TEE HUK through the System PTA. No device-specific unlock secret or sealed \
key blob is stored on disk."

LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/BSD-2-Clause;md5=cb641bc04cda31daea161b1bc15da69f"

DEPENDS = "optee-os-tadevkit python3-cryptography-native optee-client"

inherit python3native

SRC_URI = " \
    file://Makefile \
    file://sub.mk \
    file://frdm_luks_ta.c \
    file://frdm_luks_ta.h \
    file://user_ta_header_defines.h \
    file://frdm_luks_pass.c \
"

S = "${UNPACKDIR}"
B = "${WORKDIR}/build"

# Must match BINARY in the Makefile and TA_FRDM_LUKS_UUID in the shared header.
# Changing the UUID changes the device-derived credential.
TA_UUID = "62d8c105-bc79-4657-bce0-2debeed93fbb"

TA_DEV_KIT_DIR = "${STAGING_INCDIR}/optee/export-user_ta"

# This BSP's TA dev kit asks the cross compiler for libgcc. Without the recipe
# sysroot the compiler returns only "libgcc.a", and the TA link then fails.
EXTRA_OEMAKE = " \
    TA_DEV_KIT_DIR=${TA_DEV_KIT_DIR} \
    CROSS_COMPILE=${HOST_PREFIX} \
    LIBGCC_LOCATE_CFLAGS=--sysroot=${STAGING_DIR_HOST} \
    O=${B} \
"

do_compile() {
    oe_runmake -C ${S}

    ${CC} ${CFLAGS} ${CPPFLAGS} -I${S} \
        -o ${B}/frdm-luks-pass ${S}/frdm_luks_pass.c ${LDFLAGS} -lteec
}

do_compile[cleandirs] = "${B}"

do_install() {
    # optee-os embeds the stripped ELF as an early TA. The REE-FS .ta image
    # normally loaded through tee-supplicant is not needed for this boot path.
    install -D -m 0644 ${B}/${TA_UUID}.stripped.elf \
        ${D}${datadir}/early-ta/${TA_UUID}.stripped.elf

    install -D -m 0755 ${B}/frdm-luks-pass \
        ${D}${bindir}/frdm-luks-pass
}

# Keep the early-TA ELF out of the runtime frdm-luks package. It is a build
# input for optee-os, while the initramfs needs only the client.
PACKAGES =+ "${PN}-earlyta"
FILES:${PN}-earlyta = "${datadir}/early-ta/${TA_UUID}.stripped.elf"

# The TA is built against the machine-specific OP-TEE TA dev kit.
PACKAGE_ARCH = "${MACHINE_ARCH}"
