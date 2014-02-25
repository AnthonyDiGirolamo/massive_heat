#include <stdio.h>
#include <stdlib.h>

typedef struct TestCase {
	char* name;
	struct TestCase* next;
} TestCase;

typedef struct TestSuite {
	char* name;
	struct TestCase* first;
	struct TestCase* last;
} TestSuite;

void append_test_case(TestSuite* ts, char* case_name) {
	TestCase* tc = (TestCase*)malloc(sizeof(TestCase));
	tc->name = case_name;
	tc->next = NULL;

	if (ts->first == NULL) {
		// Assume ts is empty
		ts->first = tc;
		ts->last = tc;
	} else {
		// Appending to nonempty ts
		ts->last->next = tc;
		ts->last = tc;
	}
}

TestSuite* create_test_suite(char* name) {
	TestSuite* ts = (TestSuite*)malloc(sizeof(TestSuite));
	ts->name = name;
	ts->first = NULL;
	ts->last = NULL;
}

#define iterate_suite(ts, method)\
	do {\
		TestCase* tc = ts->first;\
		while(tc != NULL) {\
			method(tc);\
			tc = tc->next;\
		}\
	} while(0);

void print_test_name(TestCase* tc) {
	printf("We have a test named '%s'\n", tc->name);
}

int main(int argc, const char** argv) {
	TestSuite* ts = create_test_suite("Euler's Method Man");
	append_test_case(ts,"is a totally A-stable gangsta");
	append_test_case(ts,"knows where the party at");
	iterate_suite(ts, print_test_name);
	return 0;
}
