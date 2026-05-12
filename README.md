# holbertonschool-simple_shell
# Simple Shell
Custom UNIX shell.

## Build
`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`

## Usage
- **Interactive:** `./hsh`
- **Pipe:** `echo "ls" | ./hsh`

## Errors
Matches `sh` format: `argv[0]: line: command: not found`
