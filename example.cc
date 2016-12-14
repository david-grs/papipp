#include "papipp.h"

#include <vector>
#include <algorithm>
#include <iostream>

template <papi::event_code Event>
std::ostream& operator<<(std::ostream& os, papi::event<Event> event)
{
    return os << event.name() << ": " << event.counter();
}

int main()
{
    papi::event_set<PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_BR_MSP, PAPI_L1_DCM> events;
    events.start_counters();

    std::vector<int> v = {{11, 7, 5, 3, 1}};
    std::sort(std::begin(v), std::end(v));

    events.stop_counters();

    std::cout << events.at<0>() << "\n"
              << events.at<1>() << "\n"
              << events.at<2>() << "\n"
              << events.at<3>() << std::endl;

    return 0;
}
