#include <stdint.h>
#include <stdbool.h>

#include "psa/client.h"
#include "psa/error.h"

#include <zephyr/ipc/ipc_service.h>

#include <sdfw_psa_ipc_service.h>

static struct ipc_ept ept;

static int send(const void *data, size_t len)
{
	return ipc_service_send(&ept, data, len);
}

BUILD_ASSERT(4 == sizeof(psa_handle_t));
BUILD_ASSERT(4 == sizeof(int32_t));
BUILD_ASSERT(4 == sizeof(const psa_invec *));
BUILD_ASSERT(4 == sizeof(size_t));
BUILD_ASSERT(4 == sizeof(psa_outvec *));
BUILD_ASSERT(4 == sizeof(psa_status_t *));

psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec, size_t in_len,
                      psa_outvec *out_vec, size_t out_len)
{
	/* We have no need for this at this time */
	ARG_UNUSED(type);

	if(in_len > PSA_MAX_IOVEC ||
	   out_len > PSA_MAX_IOVEC) {
		return PSA_ERROR_PROGRAMMER_ERROR;
	}

    int32_t ret;
    psa_status_t status = PSA_ERROR_COMMUNICATION_FAILURE;

	uint32_t ipc_service_buf[SDFW_PSA_IPC_DATA_LEN];

	ipc_service_buf[0] = handle; /* i.e. TFM_CRYPTO_HANDLE defined to 0x40000100U */
	ipc_service_buf[1] = (uint32_t)in_vec;
	ipc_service_buf[2] = in_len;
	ipc_service_buf[3] = (uint32_t)out_vec;
	ipc_service_buf[4] = out_len;
	ipc_service_buf[5] = (uint32_t)&status;

	ret = send(ipc_service_buf, 7 * sizeof(uint32_t));

	if(ret != 0) {
		return PSA_ERROR_COMMUNICATION_FAILURE;
	}

    return status;
}
