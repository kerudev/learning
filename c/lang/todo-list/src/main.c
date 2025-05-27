#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "task.h"
#include "csv.h"

int main() {
    int buf_size = CONTENT_SIZE;

    printf("Write the content of your task (%d chars max): ", buf_size - 1);
    
    char content[buf_size];
    if (fgets(content, buf_size, stdin) == NULL) {
        printf("Error while reading stdin.\n");
        return 1;
    }

    size_t len = strlen(content);
    if (len > 0 && content[len - 1] == '\n') {
        content[len - 1] = '\0';
    } else {
        printf("Content was too long. Truncated to %d chars.\n", buf_size - 1);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    Task task = task_new(content);
    
    char str[50];
    sprintf(str, "%d,%s,%d\n", task.id, task.content, task.checked);

    if (csv_write("tasks.csv", str) == CSV_ERROR) {
        return 1;
    }

    return 0;
}
