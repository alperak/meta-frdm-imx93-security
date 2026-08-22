/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Interface shared between the FRDM i.MX93 LUKS unlock TA and its client.
 */

#ifndef FRDM_LUKS_TA_H
#define FRDM_LUKS_TA_H

/*
 * The System PTA includes the calling TA's UUID in the derivation context.
 * Changing this UUID therefore changes the derived secret and requires a
 * migration plan for any volume provisioned with the old value.
 */
#define TA_FRDM_LUKS_UUID                                  \
    {                                                      \
        0x62d8c105, 0xbc79, 0x4657,                        \
        {                                                  \
            0xbc, 0xe0, 0x2d, 0xeb,                        \
            0xee, 0xd9, 0x3f, 0xbb                         \
        }                                                  \
    }

/*
 * Derive the unlock secret for one volume.
 *
 * [in]  memref[0]  Context identifying the volume.
 * [out] memref[1]  Exactly FRDM_LUKS_DERIVED_SECRET_SIZE bytes.
 *
 * The client later hex-encodes this binary value to form the LUKS passphrase.
 * Returns TEE_ERROR_ACCESS_DENIED after the lock command has been issued.
 */
#define TA_FRDM_LUKS_CMD_DERIVE_SECRET 0

/*
 * Refuse further derivations for the lifetime of this TA instance.
 * There is no command that clears the lock, by design.
 */
#define TA_FRDM_LUKS_CMD_LOCK          1

/* The System PTA limits a derived key to 32 bytes. */
#define FRDM_LUKS_DERIVED_SECRET_SIZE  32

/* Local limit for the caller-supplied context. */
#define FRDM_LUKS_CONTEXT_MAX_SIZE     256

#endif /* FRDM_LUKS_TA_H */
