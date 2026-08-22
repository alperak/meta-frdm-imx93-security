# Prepare the TA dev kit for early TAs. OP-TEE documents this as optional, but
# enabling it here avoids rebuilding the user-TA libraries during embedding.
EXTRA_OEMAKE:append = " CFG_EARLY_TA=y"
