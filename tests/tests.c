# include "tests.h"

static t_global	*data = NULL;

/*
This function is called before each test case runs. 
It's used to initialize data structures, allocate memory, 
or set up any prerequisites necessary for the test to run under the expected conditions. 
Essentially, it prepares the test environment for each test.
*/
void setUp(void) {
    data = init_data();
    }
/*
tearDown function: This function is called after each test case completes. 
Its primary purpose is to clean up the environment after a test has run.
 This might include freeing allocated memory, resetting hardware states, or clearing any modifications made during the test. 
It helps ensure that one test case does not contaminate the environment for the next one, maintaining isolation between tests.
*/
void tearDown(void) {     
    if (data->cur_cwd_path != NULL) {
        free(data->cur_cwd_path);
    }
    free(data);
    data = NULL;
}


void test_copy_env_SuccessfullyCopiesEnvironmentVariables(void) {
    copy_env(&data);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_copy_env_SuccessfullyCopiesEnvironmentVariables);
    return UNITY_END();
}