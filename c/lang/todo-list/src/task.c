#include <string.h>
#include "task.h"

Task task_new(char *content) {
    Task task;
    task.id = 1;
    task.checked = 0;
    strcpy(task.content, content);
    return task;
}

Task task_check(Task task) {
    task.checked = 1;
    return task;
}

Task task_uncheck(Task task) {
    task.checked = 0;
    return task;
}
