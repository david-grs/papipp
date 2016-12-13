#include "papipp.h"

#include <gtest/gtest.h>

using namespace papi;

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

    c.start_counters();
    std::cout << "test" << std::endl;
    c.stop_counters();

    EXPECT_GE(c.at<0>().counter(), 1);
    EXPECT_GE(c.at<1>().counter(), 1);
}
