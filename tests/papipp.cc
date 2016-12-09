#include "papipp.h"

#include <gtest/gtest.h>

TEST(papipp, Basic)
{
  papi::event_set<PAPI_L1_DCM, PAPI_L2_DCM> l;
  papi::cache_events c;
}

