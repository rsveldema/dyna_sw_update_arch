#include "dyn_update.h"


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#include "UpdateLoader.h"

namespace DUpdate
{

UpdateLoader::UpdateLoader(const std::string& so_filename)
{
    m_handle = dlopen(so_filename.c_str(), RTLD_LAZY);
    assert(m_handle);
}

IFunctionPtr::ifunc_t UpdateLoader::get(const std::string& name)
{
    auto func = dlsym(m_handle, name.c_str());
    assert(func);
    return (IFunctionPtr::ifunc_t)func;
}

} // namespace DUpdate
