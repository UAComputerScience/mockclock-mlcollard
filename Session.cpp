/*
    Session.cpp

    Example of dependency injection and mock objects.

    * Classes are implemented directly to focus on dependency
      injection and mock objects. In production code, all
      classes would be in separate .hpp and .cpp files.
    * The code uses the C library time.h (ctime). In production
      code, use std::chrono.
*/

#include <ctime>
#include <thread>
#include <chrono>
#include <cassert>
#include "SessionReport.hpp"

namespace {
    // Wrapper for C++ sleep as UNIX sleep()
    void sleep(int t) {
        std::this_thread::sleep_for(std::chrono::seconds(t));
    }
}

class Clock {
public:
    virtual std::time_t start() const = 0;
    virtual std::time_t stop() const = 0;
};

class TimeClock : public Clock {
public:
    std::time_t start() const {
        return stop();
    }

    std::time_t stop() const {
        return std::time(nullptr);
    }
};

class Session {
public:
    // constructor
    Session(const Clock& clock = TimeClock())
        : clock(clock), start_time(clock.start())
    {}

    // stop the session
    void stop() {
        stop_time = clock.stop();
    }

    // elapsed time in seconds of the stopped session
    std::time_t seconds() {
        return stop_time - start_time;
    }

private:
    const Clock& clock;
    std::time_t start_time;
    std::time_t stop_time;
};

class TenMinuteClock : public Clock {
public:
    std::time_t start() const {
        return 0;
    }

    std::time_t stop() const {
        return 60 * 10;
    }
};

template <int Length>
class MockClock : public Clock {
public:
    std::time_t start() const {
        return 0;
    }

    std::time_t stop() const {
        return Length;
    }
};

int main() {

    // 2-second session
    {
        Session s;
        sleep(2);
        s.stop();
        assert(SessionReport::displayTime(s.seconds()) == "00:00:02");
    }

    // 10-minute session
    {
        TenMinuteClock clock;
        Session s(clock);
        s.stop();
        assert(SessionReport::displayTime(s.seconds()) == "00:10:00");
    }

    // 10-minute session
    {
        MockClock<10 * 60> clock;
        Session s(clock);
        s.stop();
        assert(SessionReport::displayTime(s.seconds()) == "00:10:00");
    }

    return 0;
}
