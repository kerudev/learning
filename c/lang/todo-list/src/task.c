#include <string.h>
#include "task.h"

Task task_new(char *content) {
    Task task;
    task.id = 1;
    task.checked = false;
    strcpy(task.content, content);
    return task;
}

Task task_check(Task task) {
    task.checked = true;
    return task;
}

Task task_uncheck(Task task) {
    task.checked = false;
    return task;
}

// Task task_read_line() {}
