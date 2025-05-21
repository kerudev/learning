#include <string.h>
#include "task.h"

Task new_task(char content[]) {
    Task task;
    task.id = 1;
    task.checked = false;
    strcpy(task.content, content);
    return task;
}

Task check_task(Task task) {
    task.checked = true;
    return task;
}

Task uncheck_task(Task task) {
    task.checked = false;
    return task;
}
