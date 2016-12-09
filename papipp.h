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

inline std::string get_papi_event_name(int event_code)
{
    std::array<char, PAPI_MAX_STR_LEN> event_name;
    PAPI_event_code_to_name(event_code, event_name.data());

    return event_name.data();
}

using papi_counter_type = int;

template <papi_counter_type _CounterTypeT>
struct counter
{
    using value_type = long long;

    value_type value() const { return value; }

    static constexpr papi_counter_type type() { return _CounterTypeT; }
    static const std::string& name() { return s_name; }

private:
    value_type _value;

    static const std::string s_name;
};

template <papi_counter_type _CounterTypeT>
const std::string counter<_CounterTypeT>::s_name = get_papi_event_name(_CounterTypeT);


using event = int;

template <event... _EventsT>
struct event_set
{
    static constexpr std::size_t events_count = sizeof...(_EventsT);
    static_assert(events_count > 0, "at least one event has to be in the set");

    using counters_type = std::array<long long, events_count>;

    static const std::array<event, events_count>& get_event_types()
    {
        return s_events;
    }

    void start()
    {
        int ret;

        if (likely_false((ret = ::PAPI_start_counters(const_cast<int*>(s_events.data()), events_count)) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_start_counters failed with error: ") + PAPI_strerror(ret));
    }

    void stop()
    {
        int ret;

        if (likely_false((ret = PAPI_stop_counters(&m_counters[0], events_count)) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_stop_counters failed with error: ") + PAPI_strerror(ret));
    }

    const counters_type& get_counters() const
    {
        return m_counters;
    }

    template <event _EventIndexT>
    long long get_counter() const
    {
        return m_counters[_EventIndexT];
    }

    template <event _EventIndexT>
    static constexpr int get_event_type()
    {
        return s_events[_EventIndexT];
    }

    template <event _EventIndexT>
    static const std::string& get_event_name()
    {
        static_assert(_EventIndexT < events_count, "event index greater than number of events in the set");
        return s_event_names[_EventIndexT];
    }

   private:
    static constexpr const std::array<event, events_count> s_events = {{_EventsT...}};
    static const std::array<std::string, events_count> s_event_names;

    counters_type m_counters;
};

template <event... _EventsT>
constexpr const std::array<event, event_set<_EventsT...>::events_count> event_set<_EventsT...>::s_events;

template <std::size_t _SizeT>
static auto get_papi_event_names(const std::array<event, _SizeT>& events)
{
    std::array<std::string, _SizeT> ret;
    for (std::size_t i = 0; i < ret.size(); ++i)
        ret[i] = get_papi_event_name(events[i]);

    return ret;
}

template <event... _EventsT>
const std::array<std::string, event_set<_EventsT...>::events_count> event_set<_EventsT...>::s_event_names =
    get_papi_event_names(event_set<_EventsT...>::s_events);

using cache_events = event_set<PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_TCM>;
using instr_events = event_set<PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_BR_MSP>;

}
