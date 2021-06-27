#ifndef SHARED_LIB___H___
#define SHARED_LIB___H___

#include "IFunction.h"

namespace DUpdate
{

class UpdateLoader
{
  public:
    UpdateLoader(const std::string& so_filename);

    IFunctionPtr::ifunc_t get(const std::string& name);

  public:
    void* m_handle;
};

} // namespace DUpdate

#endif