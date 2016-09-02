#include "papi.h"

int main()
{
  papi::event_set<PAPI_L1_DCM, PAPI_L2_DCM> l;

  papi::cache_events c;
  return 0;
}
