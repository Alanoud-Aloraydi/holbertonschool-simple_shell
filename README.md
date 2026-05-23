# Simple Shell

A custom UNIX command-line interpreter built in C as part of the Holberton School curriculum.

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

**Interactive:**
```bash
$ ./hsh
#cisfun$ ls
#cisfun$ exit
```

**Non-interactive:**
```bash
$ echo "ls" | ./hsh
```

## Built-in Commands

| Command | Description |
|---|---|
| `exit [status]` | Exit the shell |
| `env` | Print environment variables |
| `cd [directory]` | Change directory |

## Files

| File | Description |
|---|---|
| `shell.c` | Main loop and entry point |
| `builtins.c` | Built-in command implementations |
| `env.c` | Environment variable utilities |
| `path.c` | PATH resolution for commands |
| `shell.h` | Header file and function prototypes |
| `man_1_simple_shell` | Manual page |

## Error Format

```
./hsh: 1: command_name: not found
```

## Authors

- Alanoud Aloraydi
- Dana Alharbi
