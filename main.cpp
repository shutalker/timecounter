#include "timecounter.h"
#include <cmath>


vector<int64_t> time_extract;

void exclude_outliers( const int extract_size )
{
    int64_t median              = 0;
    int64_t lower_quartile      = 0;
    int64_t upper_quartile      = 0;
    int64_t interquartile_range = 0;
    int64_t border_error        = 0;
    int64_t lower_border        = 0;
    int64_t upper_border        = 0;

    // sorting in ascending order
    sort(time_extract.begin(), time_extract.end());

    // calculating median and quartiles of sorted vector
    if( extract_size % 2 == 0 )
    {
        median = ( time_extract[extract_size / 2 - 1] + 
                   time_extract[extract_size / 2]     ) / 2;

        lower_quartile = time_extract[extract_size / 4];
        upper_quartile = time_extract[extract_size / 4 + extract_size / 2]; 
    }
    else
    {
        median = time_extract[extract_size / 2];

        lower_quartile = ( time_extract[extract_size / 4 - 1] +
                           time_extract[extract_size / 4]     ) / 2;

        upper_quartile = ( 
            time_extract[extract_size / 4 + extract_size / 2] +
            time_extract[extract_size / 4 + extract_size / 2 + 1]
            ) / 2;
    }

    // calcualting interquartile range
    interquartile_range = upper_quartile - lower_quartile;

    // calculating outter borders
    border_error = ( interquartile_range * 3 ) / 2;

    lower_border = lower_quartile - border_error; // lower border

    if( lower_border < 0 )
        lower_border = 0;

    upper_border = upper_quartile + border_error; // upper border

    // excluding outliers
    for (auto i = 0; i < time_extract.size(); i++ )
    {
        if( time_extract[i] < lower_border
            || time_extract[i] > upper_border )
        {
            time_extract.erase( time_extract.begin() + i );
            i--;
        }
    }
}

int64_t get_average( const vector<int64_t> extract )
{
    int64_t average = 0;

    for ( auto i = 0; i < extract.size(); i++ )
    {
        average += extract[i];
    }

    average /= extract.size();

    return average;
}

int main()
{
    const string name( "test" );

    const int time_extract_size        = 10000;
    const int set_of_measurements_size = 100;
    
    // statisctics variables
    int64_t measurement_avg;
    double  measurement_deviation;

    int64_t overall_avg           = 0;

    // average for all deviations from measurements
    double  overall_avg_deviation = 0.0;              

    vector<int64_t> avg_extract;
    vector<double>  deviation_extract;

    // container of attributes for getrusage()
    rusage timestamp;

    for( auto i = 0; i < set_of_measurements_size; i++ )
    {
        measurement_avg = 0;
        measurement_deviation = 0.0; 

        for( auto j = 0; j < time_extract_size; j++ )
        {
            TimeCounter t ( name );
         
            // code for testing
            getrusage( RUSAGE_SELF, &timestamp );

            // another code for testing
/*
            // array for fetching management information base 
            // (MIB) attribute
            int          *mib;

            int cpu_freq;

            // amount of hierarchy levels for attribute name in MIB
            uint32_t mib_len = 4;
            uint32_t cpu_freq_len = sizeof( cpu_freq );

            mib = new int[mib_len];

            // convertation attribute 'dev.cpu.0.freq' to mib attribute
            // for sysctl
            if( sysctlnametomib( "dev.cpu.0.freq", mib, &mib_len ) == -1 )
            {
                perror( "Sysctlnametomib: " );
                return 1;
            }

            // fetching cpu frequency via mib
            if( sysctl( mib, mib_len, ( void * ) &cpu_freq, &cpu_freq_len,
                        NULL, 0 ) == -1 )
            {
                perror( "Sysctl: " );
                return 1;
            }

            delete [] mib;
*/
        }
    
        exclude_outliers( time_extract_size );
        measurement_avg = get_average( time_extract );

        // calculating standart deviation for measurement
        for ( auto j = 0; j < time_extract.size(); j++ )
            measurement_deviation += pow( ( time_extract[j]
                                          - measurement_avg ),
                                          2.0 );
                      
        measurement_deviation = sqrt( measurement_deviation 
                                      / time_extract.size() );
        
        avg_extract.push_back( measurement_avg );
        deviation_extract.push_back( measurement_deviation );

        cout << measurement_deviation << "\t\t" << measurement_avg << endl;

        time_extract.clear();
    }

    // calculating average for measurements averages
    overall_avg = get_average( avg_extract );

    // calculating standart deviation for deviations extract
    for ( auto i = 0; i < deviation_extract.size(); i++ )
    {
        overall_avg_deviation += deviation_extract[i];
    }

    overall_avg_deviation /= deviation_extract.size();

//  double max_extract_deviation = *max_element( deviation_extract.begin(),
//                                               deviation_extract.end() );
//  double avg_to_max_ratio = overall_avg_deviation / max_extract_deviation;

    cout << "DVT_AVG: " << overall_avg_deviation << endl;
    cout << "AVG:     " << overall_avg << endl;

    return 0;
}
