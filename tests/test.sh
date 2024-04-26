
# Mandatory Part

# Compile
# Use "make -n" to see if compilation use "-Wall -Wextra -Werror". If not, select the "invalid compilation" flag.
# minishell compiles without any errors. If not, select the flag.
# The Makefile must not re-link. If not, select the flag.


 
# Simple Command & global variables
# Execute a simple command with an absolute path like /bin/ls, or any other command without any options.
# How many global variables are used? Why? Ask the evaluated student to give you a concrete example of why it feels mandatory or logical.
# Check the global variable. This global variable cannot provide any other information or data access than the number of a received signal.
# Test an empty command.
# Test only spaces or tabs.


# Arguments
# Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes or double quotes.
# Repeat multiple times with different commands and arguments.


# echo
# Execute the echo command with or without arguments, or the -n option.
# Repeat multiple times with different arguments.


# exit
# Execute exit command with or without arguments.
# Repeat multiple times with different arguments.
# Don't forget to relaunch the minishell


# Return value of a process
# Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes and double quotes. Then execute echo $?
# Check the printed value. You can do the same in bash in order to compare the results.
# Repeat multiple times with different commands and arguments. Try some wrong commands like '/bin/ls filethatdoesntexist'
# Try anything like expr $? + $?


# Signals
# ctrl-C in an empty prompt should display a new line with a new prompt.
# ctrl-\ in an empty prompt should not do anything.
# ctrl-D in an empty prompt should quit minishell --> RELAUNCH!
# ctrl-C in a prompt after you wrote some stuff should display a new line with a new prompt.
# The buffer should be clean too. Press "Enter" to make sure nothing from the previous line is executed.
# ctrl-D in a prompt after you wrote some stuff should not do anything.
# ctrl-\ in a prompt after you wrote some stuff should not do anything.
# Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
# Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.
# Try ctrl-D after running a blocking command like cat without arguments or grep “something“.
# Repeat multiple times using different commands.


# Double Quotes
# Execute a simple command with arguments and, this time, use also double quotes (you should try to include whitespaces too).
# Try a command like : echo "cat lol.c | cat > lol.c"
# Try anything except $.


# Single Quotes
# Execute commands with single quotes as arguments.
# Try empty arguments.
# Try environment variables, whitespaces, pipes, redirection in the single quotes.
# echo '$USER' must print "$USER".
# Nothing should be interpreted.


# env
# Check if env shows you the current environment variables.


# export
# Export environment variables, create new ones and replace old ones.
# Check the result with env.


# unset
# Export environment variables, create new ones and replace old ones.
# Use unset to remove some of them.
# Check the result with env.


# cd
# Use the command cd to move the working directory and check if you are in the right directory with /bin/ls
# Repeat multiple times with working and not working cd
# Also, try '.' and '..' as arguments.


# pwd
# Use the command pwd.
# Repeat multiple times in different directories.


# Relative Path
# Execute commands but this time use a relative path.
# Repeat multiple times in different directories with a complex relative path (lots of ..).


# Environment path
# Execute commands but this time without any path (ls, wc, awk and so forth).
# Unset the $PATH and ensure commands are not working anymore.
# Set the $PATH to a multiple directory value (directory1:directory2) and ensure that directories are checked in order from left to right.


# Redirection
# Execute commands with redirections < and/or >
# Repeat multiple times with different commands and arguments and sometimes change > with >>
# Check if multiple tries of the same redirections fail.
# Test << redirection (it doesn't have to update the history).


# Pipes
# Execute commands with pipes like 'cat file | grep bla | more'
# Repeat multiple times with different commands and arguments.
# Try some wrong commands like 'ls filethatdoesntexist | grep bla | more'
# Try to mix pipes and redirections.


# Go Crazy and history
# Type a command line, then use ctrl-C and press "Enter". The buffer should be clean and there should be nothing left to execute.
# Can we navigate through history using Up and Down? Can we retry some command?
# Execute commands that should not work like 'dsbksdgbksdghsd'. Ensure minishell doesn't crash and prints an error.
# 'cat | cat | ls' should behave in a "normal way".
# Try to execute a long command with a ton of arguments.
# Have fun with that beautiful minishell and enjoy it!


# Environment variables
# Execute echo with some environment variables ($variable) as arguments.
# Check that $ is interpreted as an environment variable.
# Check that double quotes interpolate $.
# Check that USER exists. Otherwise, set it.
# echo "$USER" should print the value of the USER variable.


# Bonus
# Evaluate the bonus part if, and only if, the mandatory part has been entirely and perfectly done, and the error management handles unexpected or bad usage. In case all the mandatory points were not passed during the defense, bonus points must be totally ignored.

# And, Or
# Use &&, || and parenthesis with commands and ensure minishell behaves the same way bash does.


# Wildcard
# Use wildcards in arguments in the current working directory.


# Surprise! (or not...)
# Set the USER environment variable.
# echo "'$USER'" should print the value of the USER variable.
# echo '"$USER"' should print "$USER".





#--------------------------------------------------------------------------------------------------------------
#!/bin/bash

# Define the paths to your Minishell and the output files
MINISHELL="./minishell"
OUTPUT_MINISHELL="output_minishell.txt"
OUTPUT_BASH="output_bash.txt"
touch output_minishell.txt
touch output_bash.txt

# Valgrind leak check
LEAK_TOGGLE=0           #Set to 1 to run Valgrind, some other value for no check
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

# Colors for pretty printing
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color

check_leaks() {
    local description=$1
    local input=$2
    local valgrind_output="valgrind_output.txt"

    # Adjusted the command to run Minishell under Valgrind
    echo "$input" | $VALGRIND $MINISHELL > /dev/null 2> $valgrind_output

    # Check for the definitive phrase indicating no leaks and no errors
    if grep -q "no leaks are possible" $valgrind_output && grep -q "ERROR SUMMARY: 0 errors" $valgrind_output; then
        printf "${GREEN}${BOLD}VALGRIND: NO LEAKS${NC}\n"
    else
        # Check if the term 'definitely lost' or 'indirectly lost' is found with non-zero byte counts
        if grep -E "definitely lost: [1-9]|indirectly lost: [1-9]" $valgrind_output; then
            printf "${RED}${BOLD}VALGRIND: LEAK FOUND${NC}\n"
            mkdir -p tester/leaks
            cp $valgrind_output tester/leaks/"$(echo $description | sed 's/ /_/g')_valgrind_output.txt"
            printf "Valgrind output saved to ${BOLD}tester/leaks/$(echo $description | sed 's/ /_/g')_valgrind_output.txt${NC}\n"
        else
            printf "${YELLOW}${BOLD}VALGRIND: No definite leaks but review manually recommended${NC}\n"
        fi
    fi

    # Optionally, remove the Valgrind output file after checks
    rm -f $valgrind_output
}

# Function to run a test
run_test() {
    local description=$1
    local input=$2

    printf "${YELLOW}${BOLD}\n============ TEST: $description ($input)==============\n${NC}"

    # Run the command in Minishell and capture the output
    #echo "$input" | $MINISHELL > $OUTPUT_MINISHELL 2>&1

    # Run the same command in Bash and capture the output
    echo "$input" | bash > $OUTPUT_BASH 2>&1

    # Compare the outputs
    # if diff $OUTPUT_MINISHELL $OUTPUT_BASH > /dev/null; then
    #     printf "${GREEN}${BOLD}SUCCESS${NC}: Minishell's output matches Bash.\n"
    # else
        # printf "${RED}${BOLD}FAILURE${NC}: Minishell's output does not match Bash:\n"
        # echo "Minishell output:"
        # cat $OUTPUT_MINISHELL
        echo "Bash output:"
        cat $OUTPUT_BASH
    # fi

    # Check for leaks if enabled
    if [ "$LEAK_TOGGLE" -eq 1 ]; then
        check_leaks "$input" "$description"
    fi

    # Optionally, remove the output files after the checks
    rm -f $OUTPUT_MINISHELL $OUTPUT_BASH
}

# Example test





# Mandatory Part

# Compile
# Use "make -n" to see if compilation use "-Wall -Wextra -Werror". If not, select the "invalid compilation" flag.
# minishell compiles without any errors. If not, select the flag.
# The Makefile must not re-link. If not, select the flag.
#make -n | grep -q " -Wall -Wextra -Werror " && echo "valid compilation" || echo "invalid compilation"



# Simple Command & global variables
# Execute a simple command with an absolute path like /bin/ls, or any other command without any options.
# How many global variables are used? Why? Ask the evaluated student to give you a concrete example of why it feels mandatory or logical.
# Check the global variable. This global variable cannot provide any other information or data access than the number of a received signal.
# Test an empty command.
# Test only spaces or tabs.
run_test "Simple command with an absolute path" "/bin/ls"
run_test "Simple command with an absolute path" "/bin/pwd"
#  these ones should give a new line/prompt
run_test "empty command" ""
run_test "only spaces" "    "
run_test "only tabs" "		"

# Arguments
# Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes or double quotes.
# Repeat multiple times with different commands and arguments.
run_test "Single command / absolute path w/ arguments" "/bin/echo Hello!"
run_test "Single command / absolute path w/ arguments" "/bin/echo Hello! How are you?"


# echo
# Execute the echo command with or without arguments, or the -n option.
# Repeat multiple times with different arguments.
# basic echo tests
run_test "echo with single word" "echo Minishell"
run_test "echo with multiple words" "echo This is Minishell"
run_test "echo with special characters" "echo !@#$%^&*()"
run_test "echo with environment variable" 'echo $HOME'
run_test "echo with single quotes" "echo 'This should display as is'"
run_test "echo with double quotes" 'echo "Double quotes here"'
# testing -n option with different scenarios (-n suppresses the newline at the end of the output.)
run_test "echo with -n and single word" "echo -n Minishell"
run_test "echo with -n and multiple words" "echo -n This is Minishell"
run_test "echo with -n, quotes, and special characters" 'echo -n "Special $chars & quotes"'
# testing echo with escape sequences
run_test "echo with escape sequences and new lines" 'echo -e "Line1\nLine2\nLine3"'
run_test "echo with tab escape sequence" 'echo -e "Column1\tColumn2\tColumn3"'
# complex echo commands
run_test "echo with combined single and double quotes" "echo 'This is a single-quoted string with a \"double-quoted string\" inside'"
run_test "echo with backslashes" 'echo "\\"Backslashes\\" and more backslashes\\\\"'


# exit
# Execute exit command with or without arguments.
# Repeat multiple times with different arguments.
# Don't forget to relaunch the minishell
run_test "exit without arguments" "exit"
run_test "exit with single argument" "exit 0"
run_test "exit with higher value" "exit 99999999"
run_test "exit with multiple arguments" "exit 0 1 2 3 4 5"
run_test "exit with string argument" "exit 'Goodbye!'"
#expected:
#exit
#bash: exit: Goodbye!: numeric argument required
bash: exit: hello: numeric argument required
run_test "exit with special characters" "exit !@#$%^&*()"
run_test "exit with environment variable" "exit $HOME"


# Return value of a process
# Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes and double quotes. 
# Then execute echo $? Check the printed value.
# You can do the same in bash in order to compare the results.
# Repeat multiple times with different commands and arguments. Try some wrong commands like '/bin/ls filethatdoesntexist'
# Try anything like expr $? + $?


# Signals
# ctrl-C in an empty prompt should display a new line with a new prompt.
# ctrl-\ in an empty prompt should not do anything.
# ctrl-D in an empty prompt should quit minishell --> RELAUNCH!
# ctrl-C in a prompt after you wrote some stuff should display a new line with a new prompt.
# The buffer should be clean too. Press "Enter" to make sure nothing from the previous line is executed.
# ctrl-D in a prompt after you wrote some stuff should not do anything.
# ctrl-\ in a prompt after you wrote some stuff should not do anything.
# Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
# Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.
# Try ctrl-D after running a blocking command like cat without arguments or grep “something“.
# Repeat multiple times using different commands.


# Double Quotes
# Execute a simple command with arguments and, this time, use also double quotes (you should try to include whitespaces too).
# Try a command like : echo "cat lol.c | cat > lol.c"
# Try anything except $.


# Single Quotes
# Execute commands with single quotes as arguments.
# Try empty arguments.
# Try environment variables, whitespaces, pipes, redirection in the single quotes.
# echo '$USER' must print "$USER".
# Nothing should be interpreted.


# env
# Check if env shows you the current environment variables.


# export
# Export environment variables, create new ones and replace old ones.
# Check the result with env.


# unset
# Export environment variables, create new ones and replace old ones.
# Use unset to remove some of them.
# Check the result with env.


# cd
# Use the command cd to move the working directory and check if you are in the right directory with /bin/ls
# Repeat multiple times with working and not working cd
# Also, try '.' and '..' as arguments.


# pwd
# Use the command pwd.
# Repeat multiple times in different directories.


# Relative Path
# Execute commands but this time use a relative path.
# Repeat multiple times in different directories with a complex relative path (lots of ..).


# Environment path
# Execute commands but this time without any path (ls, wc, awk and so forth).
# Unset the $PATH and ensure commands are not working anymore.
# Set the $PATH to a multiple directory value (directory1:directory2) and ensure that directories are checked in order from left to right.


# Redirection
# Execute commands with redirections < and/or >
# Repeat multiple times with different commands and arguments and sometimes change > with >>
# Check if multiple tries of the same redirections fail.
# Test << redirection (it doesn't have to update the history).


# Pipes
# Execute commands with pipes like 'cat file | grep bla | more'
# Repeat multiple times with different commands and arguments.
# Try some wrong commands like 'ls filethatdoesntexist | grep bla | more'
# Try to mix pipes and redirections.


# Go Crazy and history
# Type a command line, then use ctrl-C and press "Enter". The buffer should be clean and there should be nothing left to execute.
# Can we navigate through history using Up and Down? Can we retry some command?
# Execute commands that should not work like 'dsbksdgbksdghsd'. Ensure minishell doesn't crash and prints an error.
# 'cat | cat | ls' should behave in a "normal way".
# Try to execute a long command with a ton of arguments.
# Have fun with that beautiful minishell and enjoy it!


# Environment variables
# Execute echo with some environment variables ($variable) as arguments.
# Check that $ is interpreted as an environment variable.
# Check that double quotes interpolate $.
# Check that USER exists. Otherwise, set it.
# echo "$USER" should print the value of the USER variable.


# Bonus
# Evaluate the bonus part if, and only if, the mandatory part has been entirely and perfectly done, and the error management handles unexpected or bad usage. In case all the mandatory points were not passed during the defense, bonus points must be totally ignored.

# And, Or
# Use &&, || and parenthesis with commands and ensure minishell behaves the same way bash does.


# Wildcard
# Use wildcards in arguments in the current working directory.


# Surprise! (or not...)
# Set the USER environment variable.
# echo "'$USER'" should print the value of the USER variable.
# echo '"$USER"' should print "$USER".

# clean temp files
# rm -f output_minishell.txt
# rm -f output_bash.txt