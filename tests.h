#define NOPE 0
#define YUP 1
#define TEST_NAME argv[1]
#define cspec_context(description, tests) \
	int main(int argc, const char** argv) {\
		int test_exists = NOPE;\
		int test_passed = NOPE;\
		if (argc < 2) {\
			printf("Please specify a test to run\n");\
			return 1;\
		}\
		char* exit_msg = "No Message Set";\
		if (NOPE == YUP) {}\
		tests\
		if(test_exists == NOPE) {\
			printf("No test named '%s' exists\n", TEST_NAME);\
			return 1;\
		}\
		if(test_passed == NOPE) {\
			printf("%s\n", exit_msg);\
			return 1;\
		}\
		return 0;\
	}
#define cspec_it(description, body)\
	else if(strcmp(TEST_NAME,description) == 0) {\
		test_exists = YUP;\
		test_passed = YUP;\
		body\
	}
#define cspec_assert(message, expr) do { if (!(expr)) { test_passed = NOPE; exit_msg = message;}} while (0);
