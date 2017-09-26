#include "timecounter.h"


void TimeCounter::get_cpu_freq()
{
    // array for fetching management information base attribute
    int          mib[4];
    unsigned int mib_len = 4;
    unsigned int cpu_freq_len = sizeof( cpu_freq );

    // convertation attribute 'dev.cpu.0.freq' to mib attribute for sysctl
    if( sysctlnametomib( "dev.cpu.0.freq", mib, &mib_len ) == -1 )
    {
        perror( "Sysctlnametomib: " );
        error_flag = 1;
        return;
    }

    // fetching cpu frequency via mib
    if( sysctl( mib, mib_len, ( void * ) &cpu_freq, &cpu_freq_len,
                NULL, 0 ) == -1 )
    {
        perror( "Sysctl: " );
        error_flag = 1;
        return;
    }
}

TimeCounter::TimeCounter( const string &label_name,
                          TimeCounterNotify *notifier ):
    label( label_name ),
    notify( notifier ),
    cpu_freq( 0 ),
    error_flag( 0 )
{
    // define mask of cpu set current process may execute on
    cpuset_t affinity_set = CPUSET_T_INITIALIZER( 0x1 );

    // cpu set is applying to the current process
    id_t     proc_id = -1;

    // bind the process to defined cpu set
    if( cpuset_setaffinity( CPU_LEVEL_WHICH, CPU_WHICH_PID, proc_id,
                            sizeof( affinity_set ), &affinity_set ) == -1 )
    {
        perror( "Cpuset_setaffinity: " );
        error_flag = 1;
        return;
    }

    // set process priority to 20    
    if( setpriority( PRIO_PROCESS, 0, 20 ) == -1 )
    {
        perror("Setpriority: ");
        error_flag = 1;
        return;
    }

    // start timer
    getrusage( RUSAGE_SELF, &timer_start );
}

TimeCounter::~TimeCounter()
{
    // stop timer
    getrusage( RUSAGE_SELF, &timer_end );

    if( error_flag )
    {
        cerr << "Test failed" << endl;
        return;
    }

    get_cpu_freq();

    // get time ( process user mode time + process kernel mode time )
    // of timer_end in microseconds
    unsigned long long timer_diff = ( timer_end.ru_utime.tv_sec * 1000000 +
                                      timer_end.ru_utime.tv_usec +
                                      timer_end.ru_stime.tv_sec * 1000000 +
                                      timer_end.ru_stime.tv_usec );

    // get time of timer_start in microseconds and calculate time diff
    timer_diff -= ( timer_start.ru_utime.tv_sec * 1000000 +
                    timer_start.ru_utime.tv_usec +
                    timer_start.ru_stime.tv_sec * 1000000 +
                    timer_start.ru_stime.tv_usec );

    // get ticks amount
    unsigned long long ticks = timer_diff * ( unsigned long long ) cpu_freq;
    double  diff_in_msecs =  ( double ) timer_diff / ( double ) 1000;

    if( notify )
        notify->report( diff_in_msecs, ticks, &label );

    cout << label <<  ": " << diff_in_msecs << " ms; "
         << ticks << " ticks" << endl;
}
