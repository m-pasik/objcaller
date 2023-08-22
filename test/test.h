#ifndef TEST_H
#define TEST_H

#include <stdio.h>

void test1(int a, int b, int c, int d, int e, int f);

void test2(long long a, long long b, long long c, long long d, long long e, long long f);

void test3(__int128_t a, __int128_t b, __int128_t c);

void test4(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);

void test5(long long a, __int128_t b, __int128_t c, __int128_t d, long long e);

void test6(double a, double b, double c, double d, double e, double f, double g, double h);

void test7(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j);

void test8(long double a, long double b, long double c);

void test9(double a, int b, double c, int d, double e, int f, double g, int h, double i, int j,
           double k, int l, double m, int n, double o, int p, double q, int r, double s);

#endif
