DEPENDS += "frdm-luks"

# Must match TA_UUID in frdm-luks_1.0.bb and the UUID in the shared header.
FRDM_LUKS_TA_UUID = "62d8c105-bc79-4657-bce0-2debeed93fbb"

# This TA requires user TA support and the System PTA for HUK subkey derivation.
# CFG_SYSTEM_PTA depends on CFG_WITH_USER_TA, so enable both explicitly.
# EARLY_TA_PATHS below supplies the TA ELF and enables early TA embedding.
#
# Explicitly select the ELE backed HUK implementation. The generic insecure
# HUK stub returns all zeros, which would make the same TA and context derive
# the same secret on every device.
EXTRA_OEMAKE:append = " CFG_WITH_USER_TA=y CFG_SYSTEM_PTA=y CFG_EARLY_TA=y CFG_IMX_ELE=y"
EXTRA_OEMAKE:append = " EARLY_TA_PATHS=${STAGING_DATADIR}/early-ta/${FRDM_LUKS_TA_UUID}.stripped.elf"
