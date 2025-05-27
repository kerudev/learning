#ifndef TASK_H
#define TASK_H

#include <stdbool.h> 

#define CONTENT_SIZE 30

typedef struct Task {
    int id;
    char content[CONTENT_SIZE];
    bool checked;
} Task;

Task task_new(char *content);

Task task_check(Task task);

Task task_uncheck(Task task);

#endif
