Minishell <br> A minimal UNIX shell implementation for 42 School <br><br>

Overview <br> Minishell is a simplified UNIX shell developed as part of the 42 School curriculum. It aims to replicate basic shell functionalities, including command execution, input/output redirection, and pipelines. <br><br>

Features <br> ✅ Command execution with absolute or relative paths <br> ✅ Built-in cd command with error handling <br> ✅ Support for multiple commands separated by ; <br> ✅ Piping (|) to connect commands <br> ✅ Process management with fork, execve, waitpid <br> ✅ File descriptor management with dup2 and pipe <br> ✅ Robust error handling <br><br>

Installation & Usage <br> Compile using Makefile: make <br>

This will generate the minishell executable. <br>

Run minishell: ./minishell <br>

Then, you can enter commands like: <br> echo Hello World <br> ls -l | grep minishell <br> cd /tmp; pwd <br><br>

Allowed Functions <br> malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp <br><br>

Error Handling <br> Incorrect cd usage → "error: cd: bad arguments" <br> cd failure → "error: cd: cannot change directory to path" <br> System call failure (except execve and chdir) → "error: fatal" <br> execve failure → "error: cannot execute command" <br><br>

Notes <br> No wildcard expansion (*, ~, etc.) <br> No environment variable expansion ($VAR) <br> Command paths must be absolute or relative (no $PATH resolution) <br><br>

Cleaning Up <br> To remove compiled files: make clean <br><br>

Authors <br> 🛠 Developed as part of 42 School's curriculum. <br><br>

STEP BY STEP: <br><br>

Step 1: Basic Structures and Project Initialization <br> ✅ Implement the t_data structure: <br>

Contains a pointer to line (the current input line from the terminal). <br> Stores environment variables (envp) and other necessary data. <br> ✅ Write the init function: <br>

Initializes the t_data structure. <br> Copies environment variables into a custom format (e.g., a list or an array). <br> ✅ Implement the free_resources function: <br>

Frees memory allocated for the t_data structure. <br> At this stage, the program compiles, reads lines (without processing them), and exits correctly. <br><br>

Step 2: Signal Handling <br> ✅ Implement the handle_signals function: <br>

Handles Ctrl+C (SIGINT) to clear the input line instead of terminating the shell and displays a new prompt. <br> Ignores Ctrl+\ (SIGQUIT). <br> Integrates into the main loop where needed. <br> The program should now compile and correctly handle Ctrl+C and Ctrl+\ .<br><br>

Step 3: Reading Input Lines <br> ✅ Implement the read_line function: <br>

Displays a prompt and reads input from the terminal (e.g., using readline). <br> Adds the line to history if it’s not empty. <br> Stores the input line in the t_data structure. <br> ✅ Handle empty input lines: <br>

If the line is empty, the loop should continue without additional actions. <br> At this stage, the program reads user input, adds it to history, and ignores empty lines. <br><br>

Step 4: Syntax Analysis and Validation (Parser) <br> ✅ Implement the check_syntax function: <br>

Verifies input line correctness: <br> Ensures quotes (" and ') are properly closed. <br> Checks for valid metacharacters (e.g., avoids double ||, standalone &&, etc.). <br> ✅ If syntax is incorrect, the program displays an appropriate message and continues execution. <br> ✅ If syntax is valid, the line proceeds to the next processing step. <br><br>

Step 5: Line Sanitization and Normalization <br> ✅ Implement the sanitize_line function: <br>

Removes excess spaces and tab characters. <br> Replaces environment variables ($VAR) with their corresponding values. <br> Normalizes the line for easier processing. <br> At this stage, the program correctly reads and prepares lines for further execution. <br><br>

Step 6: Command Creation <br> ✅ Implement the ft_cmds_creation function: <br>

Splits the input line into commands and arguments (e.g., using ;, |, or redirection symbols). <br> Creates a structure representing commands (e.g., a linked list of commands with arguments). <br> ✅ Handle different types of metacharacters: <br>

Redirections (<, >, <<, >>). <br> Pipelines (|). <br><br>

Step 7: Executing Built-in Commands <br> ✅ Implement the check_for_builtins function to handle: <br>

echo with the -n flag. <br> cd (both relative and absolute paths). <br> pwd (without arguments). <br> export, unset, env (basic implementations). <br> exit (without arguments). <br> At this stage, basic built-in commands should work correctly. <br><br>

Step 8: Executing External Commands <br> ✅ Implement the execute_cmds function: <br>

Executes external commands (e.g., ls, cat). <br> Handles pipes (|) by creating child processes. <br> Supports input/output redirection (<, >). <br><br>

Step 9: Advanced Signal Handling <br> ✅ Improve signal handling: <br>

Adjust SIGINT and SIGQUIT behavior in child processes. <br> Ignore SIGQUIT in the main process. <br><br>

EXTRAS: <br> valgrind --suppressions=readline.supp ./minishell <br>

