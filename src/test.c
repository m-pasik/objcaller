#include "test.h"
#include <stdio.h>

void test(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j,
          int k, int l, int m, int n, int o, int p, int q, int r, int s, long double t)
{
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %Lf\n",
           a, b, c, d, e, f, g, h, i, j,
           k, l, m, n, o, p, q, r, s, t);
}

void test2(long double a)
{
    printf("%Lf\n", a);
}

void test3(__uint128_t a)
{
    printf("%llx\n", a);
}

void test4(double a, double b, double c, double d, double e, double f, double g, double h, double i,
           int k, int l, int m, int n, int o, int p, int q, int r, int s, int t)
{
    printf("%f %f %f %f %f %f %f %f %f\n", a, b, c, d, e, f, g, h, i);
    printf("%d %d %d %d %d %d %d %d %d %d\n", k, l, m, n, o, p, q, r, s, t);
}

void test5(double a, double b, double c, double d, double e, double f, double g, double h, double i)
{
    printf("%f %f %f %f %f %f %f %f %f\n", a, b, c, d, e, f, g, h, i);
}
