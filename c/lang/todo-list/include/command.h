#ifndef COMMANDS_H
#define COMMANDS_H

#include "task.h"

// Task commands
int add();
int edit(char **argv, void (*modifier)(Task *));
int drop(char **argv);

// File commands
int rm(char *file_name);

#endif
