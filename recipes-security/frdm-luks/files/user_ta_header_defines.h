/* SPDX-License-Identifier: BSD-2-Clause */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include "frdm_luks_ta.h"

#define TA_UUID TA_FRDM_LUKS_UUID

/*
 * Share one instance across sessions and keep it loaded after they close.
 * If the TA crashes, prevent OP-TEE from restarting it with the derivation
 * lock reset.
 */
#define TA_FLAGS        (TA_FLAG_SINGLE_INSTANCE | \
                         TA_FLAG_INSTANCE_KEEP_ALIVE | \
                         TA_FLAG_INSTANCE_KEEP_CRASHED)

#define TA_STACK_SIZE   (2 * 1024)
#define TA_DATA_SIZE    (8 * 1024)

#define TA_DESCRIPTION  "FRDM i.MX93 LUKS TA"

#endif /* USER_TA_HEADER_DEFINES_H */
