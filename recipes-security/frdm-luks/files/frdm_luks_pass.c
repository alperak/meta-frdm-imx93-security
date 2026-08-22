// SPDX-License-Identifier: BSD-2-Clause
/*
 * Client for the FRDM i.MX93 LUKS unlock secret TA.
 *
 * A successful derivation is printed as 64 lowercase hex characters with no
 * trailing newline.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tee_client_api.h>

#include "frdm_luks_ta.h"

static const TEEC_UUID ta_uuid = TA_FRDM_LUKS_UUID;

static int invoke(uint32_t cmd, const char *context)
{
    TEEC_Operation op = { };
    TEEC_Context ctx = { };
    TEEC_Session session = { };
    TEEC_Result res = TEEC_ERROR_GENERIC;
    uint8_t secret[FRDM_LUKS_DERIVED_SECRET_SIZE] = { };
    uint32_t err_origin = 0;
    int rc = EXIT_FAILURE;

    res = TEEC_InitializeContext(NULL, &ctx);
    if (res != TEEC_SUCCESS) {
        fprintf(stderr, "TEEC_InitializeContext failed: 0x%x\n", res);
        return EXIT_FAILURE;
    }

    res = TEEC_OpenSession(&ctx, &session, &ta_uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
    if (res != TEEC_SUCCESS) {
        fprintf(stderr, "TEEC_OpenSession failed: 0x%x (origin 0x%x)\n", res, err_origin);
        goto out_context;
    }

    /* --lock sends no parameters: op is zero initialized, and TEEC_NONE is 0. */
    if (cmd == TA_FRDM_LUKS_CMD_DERIVE_SECRET) {
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                         TEEC_MEMREF_TEMP_OUTPUT,
                                         TEEC_NONE, TEEC_NONE);

        /*
         * The context length is part of the derivation input. Do not include the
         * terminating NUL, because doing so would derive a different credential.
         */
        op.params[0].tmpref.buffer = (void *)context;
        op.params[0].tmpref.size = strlen(context);
        op.params[1].tmpref.buffer = secret;
        op.params[1].tmpref.size = sizeof(secret);
    }

    res = TEEC_InvokeCommand(&session, cmd, &op, &err_origin);
    if (res != TEEC_SUCCESS) {
        fprintf(stderr, "TEEC_InvokeCommand failed: 0x%x (origin 0x%x)\n", res, err_origin);
        goto out_session;
    }

    if (cmd == TA_FRDM_LUKS_CMD_DERIVE_SECRET) {
        if (op.params[1].tmpref.size != sizeof(secret)) {
            fprintf(stderr, "unexpected derived-secret size %zu\n", op.params[1].tmpref.size);
            goto out_session;
        }

        for (size_t i = 0; i < sizeof(secret); i++)
            printf("%02x", (unsigned int)secret[i]);

        if (fflush(stdout) == EOF) {
            fprintf(stderr, "failed to write LUKS credential\n");
            goto out_session;
        }
    }

    rc = EXIT_SUCCESS;

out_session:
    TEEC_CloseSession(&session);

out_context:
    TEEC_FinalizeContext(&ctx);

    explicit_bzero(secret, sizeof(secret));

    return rc;
}

int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1], "--lock"))
        return invoke(TA_FRDM_LUKS_CMD_LOCK, NULL);

    if (argc == 3 && !strcmp(argv[1], "--context"))
        return invoke(TA_FRDM_LUKS_CMD_DERIVE_SECRET, argv[2]);

    fprintf(stderr, "usage: %s --context <string>\n       %s --lock\n", argv[0], argv[0]);

    return EXIT_FAILURE;
}
