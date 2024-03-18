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
    if (data->cur_path != NULL) {
        free(data->cur_path);
    }
    free(data);
    data = NULL;
}
bool are_envs_equal(char **env1, char **env2) {
    int i = 0;

    // Loop through each string in the first environment array
    for (; env1[i] != NULL; i++) {
        // Check if the corresponding string in the second array matches
        // If it doesn't match, or if we've reached the end of env2 before env1, return false
        if (env2[i] == NULL || strcmp(env1[i], env2[i]) != 0) {
            return false;
        }
    }

    // Finally, ensure that both arrays are of the same length.
    // If env2 has more variables than env1, they are not equal.
    if (env2[i] != NULL) {
        return false;
    }

    // If we reach here, all strings matched and both arrays are of the same length
    return true;
}

void test_copy_env_SuccessfullyCopiesEnvironmentVariables(void) {
    copy_env(&data);
    TEST_ASSERT_TRUE(are_envs_equal(__environ, data->env));
}

void test_add_node_SuccessfullyAddsNodeToLinkedList(void) {
    t_tkn *head = NULL;
    add_node(&head, "first", 0, strlen("first"));
    add_node(&head, "second", 0, strlen("second"));
    add_node(&head, "third", 0, strlen("third"));
    TEST_ASSERT_EQUAL_STRING("first", head->content);
    TEST_ASSERT_EQUAL_STRING("second", head->next->content);
    TEST_ASSERT_EQUAL_STRING("third", head->next->next->content);
    TEST_ASSERT_NULL(head->next->next->next);
}

void test_populate_hashtable_SuccessfullyAddsTokenToHashtable(void) {
    t_tkn *hashtable[TABLE_SIZE];
    init_hashtable(&hashtable);
    data->usr_input = strdup("first second third");
    populate_hashtable(&data, 0, strlen("first"));
    populate_hashtable(&data, 6, strlen("second"));
    populate_hashtable(&data, 13, strlen("third"));
    TEST_ASSERT_EQUAL_STRING("first", data->hashtable[0]->content);
    TEST_ASSERT_EQUAL_STRING("second", data->hashtable[0]->next->content);
    TEST_ASSERT_EQUAL_STRING("third", data->hashtable[0]->next->next->content);
    TEST_ASSERT_NULL(data->hashtable[0]->next->next->next);
}

void test_handle_input_SuccessfullyHandlesSimpleInput(void) {
    // data->usr_input = strdup("cnp joto");
    // handle_input(&data);
    // TEST_ASSERT_EQUAL_STRING("cnp", data->hashtable[0]->content);
    // ft_memdel(data->usr_input);
}

void test_handle_input_SuccessfullyHandlesInputWithQuotes(void) {
    // data->usr_input = strdup("\"ls -l\"");
    // handle_input(&data);
    // TEST_ASSERT_EQUAL_STRING("ls -l", data->hashtable[0]->content);
    // ft_memdel(data->usr_input);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_copy_env_SuccessfullyCopiesEnvironmentVariables);
    RUN_TEST(test_add_node_SuccessfullyAddsNodeToLinkedList);
    RUN_TEST(test_populate_hashtable_SuccessfullyAddsTokenToHashtable);
    RUN_TEST(test_handle_input_SuccessfullyHandlesSimpleInput);
    RUN_TEST(test_handle_input_SuccessfullyHandlesInputWithQuotes);
    return UNITY_END();
}