// SPDX-License-Identifier: BSD-2-Clause
/*
 * FRDM i.MX93 LUKS unlock-secret TA.
 *
 * Derives a device-specific secret through OP-TEE's System PTA. On the i.MX93,
 * the OP-TEE HUK used by that path is rooted in the EdgeLock Enclave.
 */

#include <inttypes.h>
#include <pta_system.h>
#include <stdbool.h>
#include <string.h>
#include <string_ext.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "frdm_luks_ta.h"

static const TEE_UUID system_uuid = PTA_SYSTEM_UUID;

/*
 * Set by TA_FRDM_LUKS_CMD_LOCK and never cleared by a command.
 * SINGLE_INSTANCE and INSTANCE_KEEP_ALIVE preserve it across client sessions.
 */
static bool derivation_locked;

static TEE_Result derive_secret(uint32_t param_types,
                                TEE_Param params[TEE_NUM_PARAMS])
{
    const uint32_t exp_param_types =
        TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                        TEE_PARAM_TYPE_MEMREF_OUTPUT,
                        TEE_PARAM_TYPE_NONE,
                        TEE_PARAM_TYPE_NONE);
    TEE_TASessionHandle session = TEE_HANDLE_NULL;
    TEE_Param pta_params[TEE_NUM_PARAMS] = { };
    uint8_t secret[FRDM_LUKS_DERIVED_SECRET_SIZE] = { };
    TEE_Result res = TEE_ERROR_GENERIC;
    uint32_t ret_origin = 0;

    if (param_types != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    if (derivation_locked) {
        EMSG("Derivation is locked");
        return TEE_ERROR_ACCESS_DENIED;
    }

    if (!params[0].memref.buffer ||
        !params[0].memref.size ||
        params[0].memref.size > FRDM_LUKS_CONTEXT_MAX_SIZE)
        return TEE_ERROR_BAD_PARAMETERS;

    /*
     * GlobalPlatform convention for a short output buffer: report the
     * required size and let the caller retry with a large enough one.
     */
    if (!params[1].memref.buffer ||
        params[1].memref.size < sizeof(secret)) {
        params[1].memref.size = sizeof(secret);
        return TEE_ERROR_SHORT_BUFFER;
    }

    res = TEE_OpenTASession(&system_uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
                            &session, &ret_origin);
    if (res != TEE_SUCCESS) {
        EMSG("Cannot open a session to the System PTA: %#" PRIx32, res);
        goto out;
    }

    /*
     * The System PTA requires the derived-key output to reside in secure
     * memory. Derive into a TA-owned buffer, then copy the result to the
     * normal-world caller.
     */
    pta_params[0].memref.buffer = params[0].memref.buffer;
    pta_params[0].memref.size = params[0].memref.size;
    pta_params[1].memref.buffer = secret;
    pta_params[1].memref.size = sizeof(secret);

    res = TEE_InvokeTACommand(session, TEE_TIMEOUT_INFINITE,
                              PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY,
                              exp_param_types, pta_params, &ret_origin);

    TEE_CloseTASession(session);

    if (res != TEE_SUCCESS) {
        EMSG("PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY failed: %#" PRIx32, res);
        goto out;
    }

    memcpy(params[1].memref.buffer, secret, sizeof(secret));
    params[1].memref.size = sizeof(secret);

    /*
     * Reached on the success path and on every failure after this point, so
     * the TA-owned copy is always wiped. memzero_explicit is used instead of
     * memset because it is written to survive dead store elimination, which
     * a plain memset on a dying local does not.
     */
out:
    memzero_explicit(secret, sizeof(secret));
    return res;
}

/*
 * There is no unlock command, by design. The flag clears only when the TA
 * instance is destroyed, which does not happen while the instance is kept
 * alive for the rest of the boot.
 */
static TEE_Result lock_derivation(uint32_t param_types)
{
    if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;

    derivation_locked = true;
    IMSG("Derivation locked");

    return TEE_SUCCESS;
}

/*
 * OP-TEE requires these four entry points. This TA holds no per-session
 * state, so nothing has to be set up or torn down.
 */
TEE_Result TA_CreateEntryPoint(void)
{
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types __unused,
                                    TEE_Param params[TEE_NUM_PARAMS] __unused,
                                    void **session __unused)
{
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session __unused)
{
}

TEE_Result TA_InvokeCommandEntryPoint(void *session __unused, uint32_t cmd,
                                      uint32_t param_types,
                                      TEE_Param params[TEE_NUM_PARAMS])
{
    switch (cmd) {
    case TA_FRDM_LUKS_CMD_DERIVE_SECRET:
        return derive_secret(param_types, params);

    case TA_FRDM_LUKS_CMD_LOCK:
        return lock_derivation(param_types);

    default:
        EMSG("Command ID %#" PRIx32 " is not supported", cmd);
        return TEE_ERROR_NOT_SUPPORTED;
    }
}
