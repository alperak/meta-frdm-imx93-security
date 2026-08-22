DEPENDS += "frdm-luks"

# Must match TA_UUID in frdm-luks_1.0.bb and the UUID in the shared header.
FRDM_LUKS_TA_UUID = "62d8c105-bc79-4657-bce0-2debeed93fbb"

# CFG_IMX_ELE provides the HUK from the ELE. OP-TEE's default HUK is all zeros,
# so without it every board would derive the same credential. Without
# CFG_WITH_USER_TA, OP-TEE disables the System PTA with only a build warning
# and does not embed early TAs.
EXTRA_OEMAKE:append = " CFG_WITH_USER_TA=y CFG_SYSTEM_PTA=y CFG_EARLY_TA=y CFG_IMX_ELE=y"
EXTRA_OEMAKE:append = " EARLY_TA_PATHS=${STAGING_DATADIR}/early-ta/${FRDM_LUKS_TA_UUID}.stripped.elf"
