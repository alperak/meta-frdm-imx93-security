# global-incdirs-y puts this directory on the include path so the dev kit's
# own user_ta_header.c finds user_ta_header_defines.h.
global-incdirs-y += .
srcs-y += frdm_luks_ta.c
