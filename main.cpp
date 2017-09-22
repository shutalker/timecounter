#include "timecounter.h"


int main()
{
    const string label( "test" );
    long long u = 0;

    TimeCounter t( label );

    // testing code
    for( auto i = 0; i < 10000; i++ )
    {
        for( auto j = 0; j < 100000; j++ )
        {
            if( u >= 0 )
                u += i * j;
            else
                u -= i * j;
            
            u = -u;
        }
    }

    return 0;
}
