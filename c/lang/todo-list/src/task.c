#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "task.h"

Task task_new(char *content) {
    Task task;

    task.id = 1;
    strcpy(task.content, content);
    task.checked = 0;

    return task;
}

void task_check(Task *task) {
    if (task->checked) {
        printf("Task %d was already checked\n", task->id);
        return;
    }

    printf("Checking task %d\n", task->id);
    task->checked = 1;
}

void task_uncheck(Task *task) {
    if (!task->checked) {
        printf("Task %d was already unchecked\n", task->id);
        return;
    }

    printf("Unchecking task %d\n", task->id);
    task->checked = 0;
}

void task_drop(Task *task) {
    if (task->checked) {
        printf("Dropping task %d\n", task->id);
        task->id = -1;
        return;
    }

    printf("Task %d must be checked to be dropped\n", task->id);
}

Task task_from_line(char *line) {
    Task task;

    char *id = strtok(line, ",");
    char *content = strtok(NULL, ",");
    char *checked = strtok(NULL, ",");

    task.id = atoi(id);
    strcpy(task.content, content);
    task.checked = atoi(checked);

    return task;
}

void task_to_line(Task task, char *str) {
    sprintf(str, "%d,%s,%d", task.id, task.content, task.checked);
}
