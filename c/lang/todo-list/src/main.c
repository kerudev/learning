#include <stdio.h>
#include <string.h>

#include "task.h"
#include "command.h"

int help() {
    printf("This is a simple todo-list written in C, just to learn the basics of the language.\n");
    printf("\n");
    printf("The available commands are listed below:\n");
    printf("- add       write a task description to create a new task.\n");
    printf("- check     marks a task as checked.\n");
    printf("- uncheck   marks a task as unchecked.\n");
    printf("- drop      drops a task if it is checked.\n");
    printf("- remove    deletes the tasks file.\n");

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Provide at least one argument.\n");
        return 1;
    }    

    char *command = argv[1];
    int exit_code = 0;

    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        return help();
    }

    if (strcmp(command, "add") == 0) {
        exit_code = add();
    }
    else if (strcmp(command, "check") == 0) {
        exit_code = edit(argv, task_check);
    }
    else if (strcmp(command, "uncheck") == 0) {
        exit_code = edit(argv, task_uncheck);
    }
    else if (strcmp(command, "drop") == 0) {
        exit_code = edit(argv, task_drop);
    }
    else if (strcmp(command, "remove") == 0) {
        exit_code = rm("tasks.csv");
    }
    else {
        printf("The command %s doesn't exist. Please use -h or --help.\n", command);
        exit_code = 1;
    }
   
    return exit_code;
}
