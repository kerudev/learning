# c/lang/todo-list

In this project I learned:
- How to organize code (.c and .h files).
- How to manage files (reading, writing, counting lines...).
- How to load content of a file to a struct.
- How to use simple functions like `strdup`, `strtok`, `sprintf`, `atoi`... 
- How to take arguments via `int argc, char **argv`.
- How to use pointers and references.
- How to pass functions as arguments.
- How to use `gdb`.

Compile with `make all`. The help command is `./build/todo --help`.

Run with `./build/todo [COMMAND]`:
- `add`       write a task description to create a new task
- `check`     marks a task as checked
- `uncheck`   marks a task as unchecked
- `drop`      drops a task if it is checked
- `remove`    deletes the tasks file
