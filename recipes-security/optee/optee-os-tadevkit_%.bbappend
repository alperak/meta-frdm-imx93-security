# Prepare the TA dev kit for early TAs. This setting is optional, but avoids
# rebuilding the user TA libraries when OP-TEE later embeds the TA.
EXTRA_OEMAKE:append = " CFG_EARLY_TA=y"
