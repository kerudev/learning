#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "task.h"
#include "csv.h"

int main() {
    int buf_size = TASK_CONTENT_SIZE;

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
    int id = task.id;

    size_t total = 0;
    char **lines = csv_read_lines("tasks.csv", &total);

    if (total != 0) {
        char *last = strdup(lines[total - 1]);
        char *id_str = strtok(last, ",");
        id = atoi(id_str) + 1;
    }

    char str[CSV_LINE_SIZE];
    sprintf(str, "%d,%s,%d\n", id, task.content, task.checked);

    if (csv_write("tasks.csv", str) == CSV_ERROR) {
        return 1;
    }

    for (size_t i = 0; i < total; i++) {
        printf("%s\n", lines[i]);
    }

    return 0;
}
