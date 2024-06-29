#include "hal.h"

const char *hal_errToA(hal_err_code_t err) {
    switch (err) {
        case Err_Success:return "success";
        case Err_GeneralError:return "general error";
        case Err_InvalidParameter:return "invalid parameter";
        case Err_OutOfMemory:return "out of memory";
        case Err_Timeout:return "timeout";
        default:return "unknown error";
    }
}