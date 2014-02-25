#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

suspect_context("Euler's Method",
	suspect_test("Predicts y_{n+1} to within the expected context",
		suspect_assert("Expected 1 < 0", 1 < 0);
	)

	suspect_test("Does not explode",
		suspect_assert("Expected -1 < 0", -1 < 0);
	)

	suspect_test("Does, in fact, explode",
		suspect_assert("Tony is a dork", -1 < 1);
	)
)
