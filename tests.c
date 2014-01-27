#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#define test(FUNC) printf("Testing %s\n", #FUNC); FUNC ## _test();
#define assert_tol(ACTUAL, EXPECTED, TOLERANCE) assert(abs(ACTUAL - EXPECTED) < TOLERANCE)
#include "euler.h"

void euler_test() {
	double ux0[] = {0.1};
	double ux1[] = {0.7};
	double ux2[] = {0.3};
	double *u[] = {ux0, ux1, ux2};
	assert_tol(euler(1,0,u,0.01), 16.7, 100);
}


void central_difference_test() {
	double ux0[] = {0.1};
	double ux1[] = {0.7};
	double ux2[] = {0.3};
	double *u[] = {ux0, ux1, ux2};
	assert_tol(central_difference(1,0,u,0.01), -16000, 100);
}

int main(int argc, const char** argv) {
	test(euler);
	test(central_difference);
	return 0;
}
