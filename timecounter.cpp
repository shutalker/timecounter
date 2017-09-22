#include "timecounter.h"

void TimeCounter::rdtsc_start()
{
    uint32_t  tsc_high;   // high 32 bit of TSC register
    uint32_t  tsc_low;    // low 32 bit of TSC register

    asm volatile ("rdtscp"
        : "=a" (tsc_low), "=d" (tsc_high)
        :
        : "%ecx");

    tsc_start = (uint64_t)tsc_low | ( ( (uint64_t)tsc_high ) << 32 );
}

void TimeCounter::rdtsc_end()
{
    uint32_t  tsc_high;   // high 32 bit of TSC register
    uint32_t  tsc_low;    // low 32 bit of TSC register

    asm volatile ("rdtscp"
        : "=a" (tsc_low), "=d" (tsc_high)
        :
        : "%ecx");

    tsc_end = (uint64_t)tsc_low | ( ( (uint64_t)tsc_high ) << 32 );
}

TimeCounter::TimeCounter( const string &label_name ):
label( label_name )
{
    rdtsc_start();
    getrusage( RUSAGE_SELF, &timer_start );
}

TimeCounter::~TimeCounter()
{
    rdtsc_end();
    getrusage( RUSAGE_SELF, &timer_end );

    // get time ( process user mode time + process kernel mode time )
    // of timer_end in microseconds
    uint64_t timer_diff = ( timer_end.ru_utime.tv_sec * 1000000 +
                            timer_end.ru_utime.tv_usec +
                            timer_end.ru_stime.tv_sec * 1000000 +
                            timer_end.ru_stime.tv_usec );

    // get time of timer_start in microseconds and calculate time diff
    timer_diff -= ( timer_start.ru_utime.tv_sec * 1000000 +
                    timer_start.ru_utime.tv_usec +
                    timer_start.ru_stime.tv_sec * 1000000 +
                    timer_start.ru_stime.tv_usec );

    // get ticks amount
    uint64_t ticks = tsc_end - tsc_start;
    auto diff_in_msecs =  ( double ) timer_diff / ( double ) 1000;

    cout << tsc_end << "\t" << tsc_start << endl;
    cout << label <<  ": " << diff_in_msecs << " ms; " << ticks << " ticks" << endl;
}
