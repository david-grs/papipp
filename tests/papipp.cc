#include "papipp.h"

#include <gtest/gtest.h>

using namespace papipp;

TEST(event, basic)
{
    event<PAPI_L1_DCM> c;
    std::cout << c.name() << std::endl;

    EXPECT_EQ(1, sizeof(c));
    EXPECT_GE(c.name().size(), 1);
    EXPECT_EQ(PAPI_L1_DCM, c.code());
}

TEST(event_set, basic)
{
    event_set<PAPI_L1_DCM, PAPI_L2_DCM> c;

    c.start();
    std::cout << "test" << std::endl;
    c.stop();

    EXPECT_GE(c.counter<0>(), 1);
    EXPECT_GE(c.counter<1>(), 1);
}
