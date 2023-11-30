#include "lcmgcd.h"

long gcd(long m, long n)
{
        long tmp;
        while(m) { tmp = m; m = n % m; n = tmp; }       
        return n;
}
 
long lcm(long m, long n)
{
        return m / gcd(m, n) * n;
}

