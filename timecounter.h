#ifndef _TIME_COUNTER_H_
#define _TIME_COUNTER_H_

#include <iostream>        // std::cout()
#include <cstdio>          // perror()
#include <string>          // const string label
#include <sys/sysctl.h>    // sysctl(), sysctlnametomib()
#include <sys/cpuset.h>    // cpuset_setaffinity(), CPUSET_T_INITIALIZER()
#include <sys/resource.h>  // getrusage()

using namespace std;


class TimeCounter
{
    const string  label;
     TimeCounterNotify *notify;

    struct rusage timer_start;
    struct rusage timer_end;

    int cpu_freq;

    // tracing error occuiring
    int error_flag;

public:
    TimeCounter( const string &label_name,  TimeCounterNotify *notifier);
    ~TimeCounter();

    // current cpu frequency getter
    void get_cpu_freq();
};

#endif  // _TIME_COUNTER_H_
