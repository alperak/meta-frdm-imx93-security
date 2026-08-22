# frdm-luks provides the stripped ELF embedded below.
DEPENDS += "frdm-luks"

# Must match TA_UUID in frdm-luks_1.0.bb and the UUID in the shared header.
FRDM_LUKS_TA_UUID = "62d8c105-bc79-4657-bce0-2debeed93fbb"

# EARLY_TA_PATHS embeds the TA into the OP-TEE image. Setting it also enables
# CFG_EARLY_TA and, through that, CFG_EMBEDDED_TS.
EXTRA_OEMAKE:append = " EARLY_TA_PATHS=${STAGING_DATADIR}/early-ta/${FRDM_LUKS_TA_UUID}.stripped.elf"
