#ifndef UPDATE_INFO___H___
#define UPDATE_INFO___H___

#include "IFunctionPtr.h"

struct UpdateInfo
{
    const char* name;
    const char* prototype;
    DUpdate::IFunctionPtr::ifunc_t func;
};

#define UPDATE_INFO_OBJECT_NAME  __update_info__

#define DO_STRINGIFY(X) #X

#define UPDATE_INFO_OBJECT_STRING  DO_STRINGIFY(__update_info__)


#endif