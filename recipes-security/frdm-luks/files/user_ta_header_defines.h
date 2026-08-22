/* SPDX-License-Identifier: BSD-2-Clause */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include "frdm_luks_ta.h"

#define TA_UUID TA_FRDM_LUKS_UUID

/*
 * One instance for the whole boot, so neither a new session nor a crash can
 * reset the derivation lock.
 */
#define TA_FLAGS        (TA_FLAG_SINGLE_INSTANCE | \
                         TA_FLAG_INSTANCE_KEEP_ALIVE | \
                         TA_FLAG_INSTANCE_KEEP_CRASHED)

#define TA_STACK_SIZE   (2 * 1024)
#define TA_DATA_SIZE    (8 * 1024)

#define TA_DESCRIPTION  "FRDM i.MX93 LUKS unlock-secret derivation"

#endif /* USER_TA_HEADER_DEFINES_H */
