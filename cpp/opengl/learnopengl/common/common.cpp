#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>

#include "common.hpp"

char argument[4];

int process_args(int argc, char const *argv[], const char *args[], size_t args_size) {
    if (argc < 2) {
        printf("Please provide an argument.\n");
        printf("Use --help\n");
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        return -1;
    }

    for (size_t i = 0; i < args_size; i++) {
        if (strcmp(command, args[i]) == 0) {
            strcpy(argument, args[i]);
            return 0;
        }
    }

    printf("Unknown argument: %s\n", argv[1]);
    printf("Use --help\n");
    return 1;
}

int is_arg(const char *arg) {
    return strcmp(argument, arg) == 0;
}

char *get_arg(char buf[]) {
    return strcpy(argument, buf);
}
