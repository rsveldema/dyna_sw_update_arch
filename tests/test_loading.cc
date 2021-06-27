#include <gtest/gtest.h>
#include <dyn_update.h>
#include "UpdateLoader.h"
#include "test_structure_changes.h"

using namespace DUpdate;

class TestLoadFunction : public testing::Test
{
};

static bool run_v1;
static bool run_v2;

void foo_V1()
{
    run_v1 = true;
    printf("foo V1\n");
}

void foo_V2()
{
    run_v2 = true;
    printf("foo V2\n");
}


TEST_F(TestLoadFunction, test_basic_bootup1)
{
    FunctionPtr func("foo", foo_V1);

    run_v1 = run_v2 = false;

    func();

    EXPECT_TRUE(run_v1);
    EXPECT_FALSE(run_v2);
    run_v1 = run_v2 = false;

    IFunctionPtr::replace("foo", foo_V2);

    EXPECT_FALSE(run_v1);
    EXPECT_FALSE(run_v2);

    func();

    EXPECT_FALSE(run_v1);
    EXPECT_TRUE(run_v2);

    FunctionPtr func2("foo", foo_V1); // should not re go to foo_V2
    func();
    EXPECT_FALSE(run_v1);
    EXPECT_TRUE(run_v2);
}

TEST_F(TestLoadFunction, test_shared_lib_loader)
{
    FunctionPtr func("foo", foo_V1);

    run_v1 = run_v2 = false;
    func();
    EXPECT_FALSE(run_v1);
    EXPECT_TRUE(run_v2);

    install_update("./build/tests/libTestLib.so");

    run_v1 = run_v2 = false;
    func();
    EXPECT_FALSE(run_v1); // false as it should call the one from the shared-lib now.
    EXPECT_FALSE(run_v2); // false too for same reason.
}
