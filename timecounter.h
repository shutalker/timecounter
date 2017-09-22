#include <iostream>
#include <string>
#include <sys/resource.h>
#include <unistd.h>

using namespace std;


class TimeCounter
{
    const string  label;

    struct rusage timer_start;
    struct rusage timer_end;

    // values of TSC register for cpu cycles counting
    uint64_t tsc_start;
    uint64_t tsc_end;

public:
    TimeCounter(const string & label_name);
    ~TimeCounter();

    void rdtsc_start(); // asm realization of RDTSC
    void rdtsc_end();
};
