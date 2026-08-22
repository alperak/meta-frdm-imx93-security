SUMMARY = "Trusted Application and client for the data partition unlock credential"

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
    file://frdm_luks_client.c \
"

S = "${UNPACKDIR}"
B = "${WORKDIR}/build"

# Must match BINARY in the Makefile and TA_FRDM_LUKS_UUID in the shared header.
TA_UUID = "62d8c105-bc79-4657-bce0-2debeed93fbb"

TA_DEV_KIT_DIR = "${STAGING_INCDIR}/optee/export-user_ta"

EXTRA_OEMAKE = " \
    TA_DEV_KIT_DIR=${TA_DEV_KIT_DIR} \
    CROSS_COMPILE=${HOST_PREFIX} \
    LIBGCC_LOCATE_CFLAGS=--sysroot=${STAGING_DIR_HOST} \
    O=${B} \
"

do_compile() {
    oe_runmake -C ${S}

    ${CC} ${CFLAGS} ${CPPFLAGS} -I${S} \
        -o ${B}/frdm-luks-client ${S}/frdm_luks_client.c ${LDFLAGS} -lteec
}

do_compile[cleandirs] = "${B}"

do_install() {
    # optee-os embeds the stripped ELF as an early TA. The REE-FS .ta image
    # normally loaded through tee-supplicant is not needed for this boot path.
    install -D -m 0644 ${B}/${TA_UUID}.stripped.elf \
        ${D}${datadir}/early-ta/${TA_UUID}.stripped.elf

    install -D -m 0755 ${B}/frdm-luks-client \
        ${D}${bindir}/frdm-luks-client
}

PACKAGES =+ "${PN}-early-ta"
FILES:${PN}-early-ta = "${datadir}/early-ta/${TA_UUID}.stripped.elf"

PACKAGE_ARCH = "${MACHINE_ARCH}"
