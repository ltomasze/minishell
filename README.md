Minishell <br>
A minimal UNIX shell implementation for 42 School

Overview
Minishell is a simplified UNIX shell developed as part of the 42 School curriculum. It aims to replicate basic shell functionalities, including command execution, input/output redirection, and pipelines.

Features
✅ Command execution with absolute or relative paths
✅ Built-in cd command with error handling
✅ Support for multiple commands separated by ;
✅ Piping (|) to connect commands
✅ Process management with fork, execve, waitpid
✅ File descriptor management with dup2 and pipe
✅ Robust error handling

Installation & Usage
Compile using Makefile: make

This will generate the minishell executable.

Run minishell: ./minishell

Then, you can enter commands like:
echo Hello World
ls -l | grep minishell
cd /tmp; pwd

Allowed Functions
malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp

Error Handling
Incorrect cd usage → "error: cd: bad arguments"
cd failure → "error: cd: cannot change directory to path"
System call failure (except execve and chdir) → "error: fatal"
execve failure → "error: cannot execute command"

Notes
No wildcard expansion (*, ~, etc.)
No environment variable expansion ($VAR)
Command paths must be absolute or relative (no $PATH resolution)

Cleaning Up
To remove compiled files: make clean

Authors
🛠 Developed as part of 42 School's curriculum.




STEP BY STEP:

Step 1: Basic Structures and Project Initialization
✅ Implement the t_data structure:

Contains a pointer to line (the current input line from the terminal).
Stores environment variables (envp) and other necessary data.
✅ Write the init function:

Initializes the t_data structure.
Copies environment variables into a custom format (e.g., a list or an array).
✅ Implement the free_resources function:

Frees memory allocated for the t_data structure.
At this stage, the program compiles, reads lines (without processing them), and exits correctly.

Step 2: Signal Handling
✅ Implement the handle_signals function:

Handles Ctrl+C (SIGINT) to clear the input line instead of terminating the shell and displays a new prompt.
Ignores Ctrl+\ (SIGQUIT).
Integrates into the main loop where needed.
The program should now compile and correctly handle Ctrl+C and Ctrl+\.

Step 3: Reading Input Lines
✅ Implement the read_line function:

Displays a prompt and reads input from the terminal (e.g., using readline).
Adds the line to history if it’s not empty.
Stores the input line in the t_data structure.
✅ Handle empty input lines:

If the line is empty, the loop should continue without additional actions.
At this stage, the program reads user input, adds it to history, and ignores empty lines.

Step 4: Syntax Analysis and Validation (Parser)
✅ Implement the check_syntax function:

Verifies input line correctness:
Ensures quotes (" and ') are properly closed.
Checks for valid metacharacters (e.g., avoids double ||, standalone &&, etc.).
✅ If syntax is incorrect, the program displays an appropriate message and continues execution.
✅ If syntax is valid, the line proceeds to the next processing step.

Step 5: Line Sanitization and Normalization
✅ Implement the sanitize_line function:

Removes excess spaces and tab characters.
Replaces environment variables ($VAR) with their corresponding values.
Normalizes the line for easier processing.
At this stage, the program correctly reads and prepares lines for further execution.

Step 6: Command Creation
✅ Implement the ft_cmds_creation function:

Splits the input line into commands and arguments (e.g., using ;, |, or redirection symbols).
Creates a structure representing commands (e.g., a linked list of commands with arguments).
✅ Handle different types of metacharacters:

Redirections (<, >, <<, >>).
Pipelines (|).
Step 7: Executing Built-in Commands
✅ Implement the check_for_builtins function to handle:

echo with the -n flag.
cd (both relative and absolute paths).
pwd (without arguments).
export, unset, env (basic implementations).
exit (without arguments).
At this stage, basic built-in commands should work correctly.

Step 8: Executing External Commands
✅ Implement the execute_cmds function:

Executes external commands (e.g., ls, cat).
Handles pipes (|) by creating child processes.
Supports input/output redirection (<, >).
Step 9: Advanced Signal Handling
✅ Improve signal handling:

Adjust SIGINT and SIGQUIT behavior in child processes.
Ignore SIGQUIT in the main process.




EXTRAS:
valgrind --suppressions=readline.supp ./minishell
