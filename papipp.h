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

namespace detail
{

template <typename ArrayT>
static constexpr int find_(int x, ArrayT& ar, int size, int i)
{
    return size == i ? -1 : (ar[i] == x ? i : find_(x, ar, size, i + 1));
}

template <int... is>
static constexpr int find(int x)
{
    constexpr const std::array<int, sizeof...(is)> ar = {{is...}};
    return find_(x, ar, sizeof...(is), 0);
}

}

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
    explicit event(papi_counter counter = {})
     : _counter(counter)
    {}

    papi_counter counter() const { return _counter; }

    static constexpr event_code code() { return _Event; }
    static const std::string& name() { return s_name; }

private:
    static const std::string s_name;

    papi_counter _counter;
};

template <event_code _Event>
const std::string event<_Event>::s_name = get_event_code_name(_Event);

template <event_code... _Events>
struct event_set
{
    static constexpr std::size_t size() { return sizeof...(_Events); }
    static_assert(size() > 0, "at least one hardware event has to be in the set");

    void start_counters()
    {
        int ret;

        if (likely_false((ret = ::PAPI_start_counters(s_events.data(), size())) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_start_counters failed with error: ") + PAPI_strerror(ret));
    }

    void stop_counters()
    {
        int ret;

        if (likely_false((ret = ::PAPI_stop_counters(_counters.data(), size())) != PAPI_OK))
            throw std::runtime_error(std::string("PAPI_stop_counters failed with error: ") + PAPI_strerror(ret));
    }

    template <std::size_t _EventIndex>
    auto at() const
    {
        static_assert(_EventIndex < size(), "event index greater than number of events in the set");

        static constexpr const std::array<event_code, sizeof...(_Events)> events = {{_Events...}};
        constexpr event_code code = events[_EventIndex];
        return event<code>(_counters[_EventIndex]);
    }

    template <event_code _EventCode>
    auto get() const
    {
        constexpr int EventIndex = detail::find<_Events...>(_EventCode);
        return at<EventIndex>();
    }

private:
    static std::array<event_code, sizeof...(_Events)> s_events;

    std::array<papi_counter, sizeof...(_Events)> _counters;
};

template <event_code... _Events>
std::array<event_code, sizeof...(_Events)> event_set<_Events...>::s_events = {{_Events...}};

}
