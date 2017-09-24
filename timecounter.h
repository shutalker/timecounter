#ifndef _TIME_COUNTER_H_
#define _TIME_COUNTER_H_

#include <iostream>        // std::cout()
#include <cstdio>          // perror()
#include <string>          // const string label
#include <sys/sysctl.h>    // sysctl(), sysctlnametomib()
#include <sys/cpuset.h>    // cpuset_setaffinity(), CPUSET_T_INITIALIZER()
#include <sys/resource.h>  // getrusage()
#include <unistd.h>        // exit()

using namespace std;


class TimeCounter
{
    const string  label;
    // TimeCounterNotify *notify;

    struct rusage timer_start;
    struct rusage timer_end;

    /* ONLY FOR TESTING RDTSC VS GETRUSAGE + CPU_FREQ */

    // values of TSC register for cpu cycles counting
    unsigned long long tsc_start;
    unsigned long long tsc_end;

    /* END TESTING BLOCK */

    int cpu_freq;

    // tracing error occuiring
    int error_flag;

public:
    TimeCounter( const string &label_name/*,  TimeCounterNotify *notify */);
    ~TimeCounter();

    /* ONLY FOR TESTING RDTSC VS GETRUSAGE + CPU_FREQ */

    // asm realization of RDTSC
    void rdtsc_start();
    void rdtsc_end();

    /* END TESTING BLOCK */

    // current cpu frequency getter
    void get_cpu_freq();
};

#endif  // _TIME_COUNTER_H_
