#ifndef TASK_H
#define TASK_H

#include <stdbool.h> 

typedef struct Task {
    int id;
    char content[50];
    bool checked;
} Task;

Task new_task(char content[]);

Task check_task(Task task);

Task uncheck_task(Task task);

#endif
