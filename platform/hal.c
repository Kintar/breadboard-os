#include "hal.h"

const char *hal_errToA(t_hal_err_code err) {
    switch (err) {
        case Success:return "success";
        case GeneralError:return "general error";
        case InvalidParameter:return "invalid parameter";
        case OutOfMemory:return "out of memory";
        case Timeout:return "timeout";
        default:return "unknown error";
    }
}