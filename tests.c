#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

cspec_context("Euler's Method",
	cspec_it("Predicts y_{n+1} to within the expected context",
		cspec_assert("Expected 1 < 0", 1 < 0)
	)

	cspec_it("Does not explode",
		cspec_assert("Expected -1 < 0", -1 < 0)
	)
)
