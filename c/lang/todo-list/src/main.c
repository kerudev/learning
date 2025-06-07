#include <stdio.h>
#include <string.h>

#include "task.h"
#include "command.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Provide at least one argument.\n");
        return 1;
    }    

    char *command = argv[1];
    int exit_code = 0;

    if (strcmp(command, "add") == 0) {
        exit_code = add();
    } else if (strcmp(command, "check") == 0) {
        exit_code = edit(argv, task_check);
    } else if (strcmp(command, "uncheck") == 0) {
        exit_code = edit(argv, task_uncheck);
    } else if (strcmp(command, "drop") == 0) {
        exit_code = edit(argv, task_drop);
    } else if (strcmp(command, "remove") == 0) {
        exit_code = rm("tasks.csv");
    } else {
        printf("The command %s doesn't exist\n", command);
        exit_code = 1;
    }
   
    return exit_code;
}
