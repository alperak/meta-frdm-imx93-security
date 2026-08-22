# The UUID is the binary name. ta_dev_kit.mk derives its output names from it,
# including the <uuid>.stripped.elf that will later be embedded as an early TA.
BINARY = 62d8c105-bc79-4657-bce0-2debeed93fbb

# Yocto exports LDFLAGS for normal userspace applications. Keep those flags out
# of the separate TA link performed by the OP-TEE TA dev kit.
LDFLAGS =

include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk
