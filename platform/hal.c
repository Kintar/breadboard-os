#include "hal.h"

const char *hal_errToA(hal_err_code_t err) {
    switch (err) {
        case HAL_ERR_SUCCESS:return "success";
        case HAL_ERR_GENERAL_ERROR:return "general error";
        case HAL_ERR_INVALID_PARAMETER:return "invalid parameter";
        case HAL_ERR_OUT_OF_MEMORY:return "out of memory";
        case HAL_ERR_TIMEOUT:return "timeout";
        default:return "unknown error";
    }
}