#ifndef TASK_H
#define TASK_H

#define TASK_CONTENT_SIZE 100

typedef struct Task {
    int id;
    char content[TASK_CONTENT_SIZE];
    int checked;
} Task;

Task task_new(char *content);

void task_check(Task *task);
void task_uncheck(Task *task);
void task_drop(Task *task);

Task task_from_line(char *line);
void task_to_line(Task task, char *str);

#endif
