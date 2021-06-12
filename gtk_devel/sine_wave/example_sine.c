#include <math.h>
#include <stdio.h>
int main(void)
{
    double  angle = 90;
    double x = angle/180 * 3.14;

    double res = sin(x);
    printf("sin(%f) = %f\n", angle, res);
    return 0;
}
