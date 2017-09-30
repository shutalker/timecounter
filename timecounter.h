#ifndef _TIME_COUNTER_H_
#define _TIME_COUNTER_H_

#include <iostream>        // std::cout()
#include <cstdio>          // perror()
#include <string>          // const string label
#include <sys/sysctl.h>    // sysctl(), sysctlnametomib()
#include <sys/cpuset.h>    // cpuset_setaffinity(), CPUSET_T_INITIALIZER()
#include <sys/resource.h>  // getrusage()
#include <unistd.h>
#include <vector>

using namespace std;


class TimeCounter
{
public:
    TimeCounter( const string &label_name
                /*, TimeCounterNotify *notifier */ );
    ~TimeCounter();

private:
    const string  label;
//  TimeCounterNotify *notify;

    rusage timer_start;
    rusage timer_end;

    int  cpu_freq;

    bool error_flag;

    // current cpu frequency getter
    void get_cpu_freq();
};

#endif  // _TIME_COUNTER_H_
