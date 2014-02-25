/* * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Automatic Test Case Iteration
 *
 * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct TestCase {
	char* name;
	struct TestCase* next;
} TestCase;

typedef struct TestSuite {
	char* name;
	struct TestCase* first;
	struct TestCase* last;
} TestSuite;

int append_test_case(TestSuite* ts, char* case_name) {
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

	return 0;
}

TestSuite* create_test_suite(char* name) {
	TestSuite* ts = (TestSuite*)malloc(sizeof(TestSuite));
	ts->name = name;
	ts->first = NULL;
	ts->last = NULL;
}

void print_test_name(TestCase* tc) {
	printf("We have a test named '%s'\n", tc->name);
}

static int num_failures = 0;
char* test_executable_path;
// ANSI Color stuff stolen gleefully from http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
// Many thanks to Andrejs Cainikovs (http://stackoverflow.com/users/147407/andrejs-cainikovs)
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
void run_test(TestCase* tc) {
	char test_command[256];
	int test_status;
	printf("\t%s ",tc->name);

	sprintf(test_command, "%s \"%s\"", test_executable_path, tc->name);
	test_status = system(test_command);
	if(test_status != 0) {
		num_failures++;
		printf(ANSI_COLOR_RED "Fail" ANSI_COLOR_RESET "\n");
	} else {
		printf(ANSI_COLOR_GREEN "Pass" ANSI_COLOR_RESET "\n");
	}
}

#define iterate_suite(ts, method)\
	do {\
		TestCase* tc = ts->first;\
		while(tc != NULL) {\
			method(tc);\
			tc = tc->next;\
		}\
	} while(0);

/* * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * suspect API
 *
 * * * * * * * * * * * * * * * * * * * * * * * * */
#define NOPE 0
#define YUP 1
#define TEST_NAME argv[1]
#define suspect_context(description, tests) \
	int main(int argc, const char** argv) {\
		int test_exists = NOPE;\
		int test_passed = NOPE;\
		int driver = NOPE;\
		TestSuite* ts = NULL;\
		if (argc < 2) {\
			printf("%s\n", description);\
			ts = create_test_suite(description);\
			test_executable_path = (char*)malloc(strlen(argv[0]) * sizeof(char));\
			strcpy(test_executable_path,argv[0]);\
			driver = YUP;\
		}\
		char* exit_msg = "No Message Set";\
		if (NOPE == YUP) {}\
		tests\
		if(driver == YUP) {\
			iterate_suite(ts,run_test);\
			printf("%d tests failed.\n",num_failures);\
			return (num_failures > 0) ? 0 : 1;\
		}\
		if(test_exists == NOPE) {\
			printf("No test named '%s' exists\n", TEST_NAME);\
			return 1;\
		}\
		if(test_passed == NOPE) {\
			printf("\t\t%s\n", exit_msg);\
			return 1;\
		}\
		return 0;\
	}
#define suspect_test(description, body)\
	else if(driver == YUP && append_test_case(ts, description) != 0) {}\
	else if(driver == NOPE && test_exists == NOPE && strcmp(TEST_NAME,description) == 0) {\
		test_exists = YUP;\
		test_passed = YUP;\
		body\
	}
#define suspect_assert(message, expr) do { if (!(expr)) { test_passed = NOPE; exit_msg = message;}} while (0);
