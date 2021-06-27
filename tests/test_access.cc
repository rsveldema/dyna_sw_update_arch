#include <gtest/gtest.h>
#include <dyn_update.h>

#include "test_structure_changes.h"

using namespace DUpdate;

class TestStartup : public testing::Test
{
};

TEST_F(TestStartup, test_basic_bootup1)
{
    auto *new_version = new MyData_V2();
    auto *old_version = &new_version->cast();

    printf("TEST-d1::::::::::: %d %d\n", new_version->d1.get(), old_version->d1.get());
    printf("TEST-d2::::::::::: %d %d\n", new_version->d2.get(), old_version->d2.get());

    ASSERT_EQ(new_version->d1, old_version->d1);
    ASSERT_EQ(new_version->d2, old_version->d2);
}

TEST_F(TestStartup, test_basic_bootup2)
{
    auto *new_version = new MyData_V3();
    auto *old_version = &new_version->cast();

    printf("TEST-d2::::::::::: %d %d\n", new_version->d2.get(), old_version->d2.get());

    ASSERT_EQ(new_version->d2, old_version->d2);
}

TEST_F(TestStartup, test_alias)
{
    auto *new_version = new MyData_V3();
    auto *old_version = &new_version->cast();

    printf("TEST-d2::::::::::: %d %d\n", new_version->d2.get(), old_version->d2.get());

    ASSERT_EQ(new_version->d2, old_version->d2);

    new_version->d2 = 42;
    ASSERT_EQ(new_version->d2, old_version->d2);
}