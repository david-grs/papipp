#include "papi.h"

int main()
{
  papi::event_set<PAPI_L1_DCM, PAPI_L2_DCM> l;
  return 0;
}
