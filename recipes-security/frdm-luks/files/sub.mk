# Source list read by the OP-TEE TA dev kit. global-incdirs-y puts this
# directory on the include path so frdm_luks_ta.h resolves.
global-incdirs-y += .
srcs-y += frdm_luks_ta.c
