#include "test.h"

void test1(int a, int b, int c, int d, int e, int f)
{
    printf("%d %d %d %d %d %d\n", a, b, c, d, e, f);
}

void test2(long long a, long long b, long long c, long long d, long long e, long long f)
{
    printf("%lld %lld %lld %lld %lld %lld\n", a, b, c, d, e, f);
}

void test3(__int128_t a, __int128_t b, __int128_t c)
{
    printf("%lld %lld %lld\n", (long long)a, (long long)b, (long long)c);
}

void test4(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
    printf("%d %d %d %d %d %d %d %d %d %d\n", a, b, c, d, e, f, g, h, i, j);
}

void test5(long long a, __int128_t b, __int128_t c, __int128_t d, long long e)
{
    printf("%lld %lld %lld %lld %lld\n", a, (long long)b, (long long)c, (long long)d, e);
}

void test6(double a, double b, double c, double d, double e, double f, double g, double h)
{
    printf("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", a, b, c, d, e, f, g, h);
}

void test7(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
{
    printf("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", a, b, c, d, e, f, g, h, i, j);
}

void test8(long double a, long double b, long double c)
{
    printf("%.1Lf %.1Lf %.1Lf\n", a, b, c);
}

void test9(double a, int b, double c, int d, double e, int f, double g, int h, double i, int j,
           double k, int l, double m, int n, double o, int p, double q, int r, double s)
{
    printf("%.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f\n",
           a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s);
}
