#ifndef TASK_H
#define TASK_H

#define TASK_CONTENT_SIZE 100

typedef struct Task {
    int id;
    char content[TASK_CONTENT_SIZE];
    int checked;
} Task;

Task task_new(char *content);
Task task_check(Task task);
Task task_uncheck(Task task);

#endif
