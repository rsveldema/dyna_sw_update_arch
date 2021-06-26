#include <gtest/gtest.h>
#include <dyn_update.h>

using namespace DUpdate;

class TestStartup : public testing::Test
{
};

TEST_F(TestStartup, test_basic_bootup)
{
    auto *new_version = new MyData_V2();
    auto *old_version = (MyData_V1*) new_version;

    ASSERT_EQ(new_version->d1, old_version->d1);
    ASSERT_EQ(new_version->d2, old_version->d2);
}