# Minishell

The **Minishell** project is part of the 42 curriculum and challenges students to build a simple Unix-like shell. The goal is to replicate basic shell functionality, allowing users to interact with the operating system through command-line inputs.

## Key Objectives

- **Command Parsing and Execution**: 
  - Interpret user input and execute corresponding system commands.
  
- **Pipes and Redirection**: 
  - Implement support for pipes (`|`) to connect processes.
  - Handle input/output redirection (`>`, `<`).

- **Environment Variables**: 
  - Support common shell commands like `echo`, `cd`, `pwd`, `export`, `unset`, and `env`.
  - Manage and modify environment variables.

- **Signal Handling**: 
  - Manage signals like:
    - `Ctrl-C`: Interrupt the current process.
    - `Ctrl-D`: End-of-file (EOF) input handling.
    - `Ctrl-\`: Quit signal.

- **Process Management**: 
  - Use system calls like `fork()`, `execve()`, and `waitpid()` to create and manage processes.

## Learning Outcomes

The **Minishell** project enhances understanding of:
- Low-level process management.
- System calls for process creation and execution.
- Shell input parsing and command execution.
- Unix-like systems and how shells interact with the OS.

By the end of the project, you'll have a deeper appreciation for how operating systems manage processes, signals, and input/output, which are critical for systems programming.
