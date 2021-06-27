#include <stdio.h>
#include <stdlib.h>

#include "UpdateInfo.h"

namespace
{
void foo_v3(bool& arg)
{
    arg = true;
    printf("accessed foo-v333 from shared lib\n");
}

} // namespace

UpdateInfo UPDATE_INFO_OBJECT_NAME[] = {{"foo", typeid(foo_v3).name(), (DUpdate::IFunctionPtr::ifunc_t)foo_v3},
                                        {nullptr, nullptr, nullptr}};
