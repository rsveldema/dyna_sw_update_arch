#include "dyn_update.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#include "UpdateLoader.h"
#include "UpdateInfo.h"


namespace DUpdate
{

bool file_exists(const std::string& filename)
{
    if (FILE* f = fopen(filename.c_str(), "r"))
    {
        fclose(f);
        return f;
    }
    return false;
}


void install_update(const std::string& so_filename)
{
    if (!file_exists(so_filename))
    {
        fprintf(stderr, "failed to find file: %s\n", so_filename.c_str());
        return;
    }
    void *handle = dlopen(so_filename.c_str(), RTLD_LAZY);
    if (! handle)
    {
        fprintf(stderr, "invalid update file: %s\n", so_filename.c_str());
        return;
    }

    auto update_info = (UpdateInfo*) dlsym(handle, UPDATE_INFO_OBJECT_STRING);
    if (! update_info)
    {
        fprintf(stderr, "invalid update file: %s (failed to find '%s')\n", so_filename.c_str(), UPDATE_INFO_OBJECT_STRING);
        return;
    }
    while (update_info->name)
    {
        auto name = update_info->name;
        auto func = update_info->func;

        IFunctionPtr::replace(name, func);
        update_info++;
    }
}

} // namespace DUpdate
