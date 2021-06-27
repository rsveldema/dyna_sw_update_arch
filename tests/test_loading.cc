#include <gtest/gtest.h>
#include <dyn_update.h>

#include "test_structure_changes.h"

using namespace DUpdate;

class TestLoadFunction : public testing::Test
{
};

void foo_V1()
{
    printf("foo V1\n");
}

void foo_V2()
{
    printf("foo V2\n");
}


TEST_F(TestLoadFunction, test_basic_bootup1)
{
    FunctionPtr func("foo", foo_V1);

    func();

    IFunctionPtr::replace("foo", foo_V2);

    func();
}
