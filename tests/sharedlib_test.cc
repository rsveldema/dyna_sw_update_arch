#include <stdio.h>
#include <stdlib.h>

#include "UpdateInfo.h"


namespace 
{
void foo_v3()
{
    printf("accessed foo-v333 from shared lib\n");
}

}


UpdateInfo UPDATE_INFO_OBJECT_NAME[] = {
    { "foo", foo_v3 },
    { nullptr, nullptr}
};

