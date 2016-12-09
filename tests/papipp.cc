#include "papipp.h"

#include <gtest/gtest.h>

using namespace papi;

TEST(papipp, Basic)
{
    //papi::event_set<PAPI_L1_DCM, PAPI_L2_DCM> l;
    //papi::cache_events c;

    //std::cout << l.get_event_name<0>() << std::endl;
}

TEST(counter, basic)
{
    counter<PAPI_L1_DCM> c;
    std::cout << c.name() << std::endl;

    EXPECT_EQ(sizeof(long long), sizeof(c));
    EXPECT_GE(c.name().size(), 1);
    EXPECT_EQ(PAPI_L1_DCM, c.event());
}
