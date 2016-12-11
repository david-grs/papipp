#pragma once

extern "C"
{
#include <papi.h>
}

#include <cstddef>
#include <array>
#include <string>

#define likely_true(x)   __builtin_expect(!!(x), 1)
#define likely_false(x)  __builtin_expect(!!(x), 0)

namespace papi
{

using event_code = int;
using papi_counter = long long;

inline std::string get_event_code_name(event_code code)
{
    std::array<char, PAPI_MAX_STR_LEN> event_name;
    ::PAPI_event_code_to_name(code, event_name.data());

    return event_name.data();
}

template <event_code _Event>
struct event
{
    static constexpr event_code code() { return _Event; }
    static const std::string& name() { return s_name; }

private:
    static const std::string s_name;
};

template <event_code _Event>
const std::string event<_Event>::s_name = get_event_code_name(_Event);

template <event_code... _Events>
struct event_set
{
    static constexpr const std::size_t events_count = sizeof...(_Events);
    static_assert(events_count > 0, "at least one counter has to be in the set");

    void start()
    {
        int ret;

        if (likely_false((ret = ::PAPI_start_counters(const_cast<int*>(s_events.data()), events_count)) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_start_counters failed with error: ") + PAPI_strerror(ret));
    }

    void stop()
    {
        int ret;

        if (likely_false((ret = PAPI_stop_counters(&_counters[0], events_count)) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_stop_counters failed with error: ") + PAPI_strerror(ret));
    }

    template <std::size_t EventIndex>
    papi_counter counter() const
    {
        static_assert(EventIndex < events_count, "event index greater than number of events in the set");
        return _counters;
    }

private:
    static constexpr const std::array<event_code, events_count> s_events = {{_Events...}};

    std::array<papi_counter, events_count> _counters;
};

template <event_code... _Events>
constexpr const std::array<event_code, event_set<_Events...>::events_count> event_set<_Events...>::s_events;


using cache_events = event_set<PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_TCM>;
using instr_events = event_set<PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_BR_MSP>;

}
