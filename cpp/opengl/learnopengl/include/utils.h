#ifndef COMMON_H
#define COMMON_H

#include <GLFW/glfw3.h>

int process_args(int argc, char const *argv[], const char *args[], size_t args_size);
int is_arg(const char *arg);
char *copy_arg(char buf[]);
char *get_arg();

#endif // COMMON_H
