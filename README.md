# C Minishell

This is a shell implementation in C that replicates bash. It supports a variety of features including quoting, redirections, pipes, environment variables handling, and several built-in commands.

## Features

- **Quoting**

  - Single quotes (`'`) to prevent interpretation of metacharacters.
  - Double quotes (`"`) where only `$` (dollar sign) is interpreted.

- **Redirections**

  - `<` for input redirection.
  - `>` for output redirection.
  - `<<` with a delimiter for input redirection until a delimiter line is reached.
  - `>>` for appending output redirection.

- **Pipes**

  - Use `|` to create a pipeline between commands.

- **Environment Variables**

  - Handle expansion of `$varname` to its value.

- **Exit Status**

  - `?` expands to the exit status of the most recently executed foreground pipeline.

- **Signal Handling**

  - `ctrl-C` interrupts the current command and displays a new prompt.
  - `ctrl-D` exits the shell.
  - `ctrl-\` has no effect.

- **Built-in Commands**
  - `echo [-n]`
  - `cd [path]`
  - `pwd`
  - `export [varname]=[value]`
  - `unset [varname]`
  - `env`
  - `exit`

## Compilation

To compile the shell, use the following command:

```bash
make
```

then run:

```bash
./minishell
```
