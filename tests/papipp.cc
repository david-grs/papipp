#include "papipp.h"

#include <gtest/gtest.h>

using namespace papi;

TEST(event, basic)
{
    event<PAPI_L1_DCM> c;
    std::cout << c.name() << std::endl;

    EXPECT_EQ(sizeof(papi_counter), sizeof(c));
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
    EXPECT_GE(c.get<PAPI_L1_DCM>().counter(), 1);
    EXPECT_GE(c.get<PAPI_L2_DCM>().counter(), 1);
}

TEST(event_set, at)
{
    event_set<PAPI_L1_DCM, PAPI_L2_DCM> c;

    EXPECT_EQ(PAPI_L1_DCM, c.at<0>().code());
    EXPECT_EQ(PAPI_L2_DCM, c.at<1>().code());
}


TEST(event_set, get)
{
    event_set<PAPI_L1_DCM, PAPI_L2_DCM> c;

    EXPECT_EQ(PAPI_L1_DCM, c.get<PAPI_L1_DCM>().code());
    EXPECT_EQ(PAPI_L2_DCM, c.get<PAPI_L2_DCM>().code());
}
