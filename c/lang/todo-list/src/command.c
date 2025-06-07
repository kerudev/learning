#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "command.h"
#include "task.h"
#include "csv.h"
#include "utils.h"

int add() {
    int buf_size = TASK_CONTENT_SIZE;

    printf("Write the content of your task (%d chars max): ", buf_size - 1);

    char content[buf_size];
    if (fgets(content, buf_size, stdin) == NULL) {
        fprintf(stderr, "Error while reading stdin.\n");
        return 1;
    }

    printf("\n");

    size_t len = strlen(content);
    if (len > 0 && content[len - 1] == '\n') {
        content[len - 1] = '\0';
    } else {
        printf("Content was too long. Truncated to %d chars.\n", buf_size - 1);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    Task task = task_new(content);

    size_t total = 0;
    char **lines = csv_read_lines("tasks.csv", &total);

    if (total != 0) {
        char last[strlen(lines[total - 1]) + 1];
        strcpy(last, lines[total - 1]);

        char *line_id = strtok(last, ",");

        task.id = atoi(line_id) + 1;
    }
    
    char line[CSV_LINE_SIZE];
    task_to_line(task, line);

    FILE *f = csv_open("tasks.csv", "a");
    int result = csv_write_line(f, line);
    fclose(f);

    if (result != 0) {
        fprintf(stderr, "Couldn't append line to file.\n");
        return result;
    }

    for (size_t i = 0; i < total; i++) printf("%s\n", lines[i]);
    printf("%s\n", line);

    return 0;
}

int edit(char **argv, void (*modifier)(Task *)) {
    if (!isdigit(argv[2])) {
        fprintf(stderr, "You must provide a valid digit to check.\n");
        return 1;
    }

    size_t total = 0;
    char **lines = csv_read_lines("tasks.csv", &total);

    char new_lines[total][CSV_LINE_SIZE];
    char *id = argv[2];

    if (strcmp(lines[0], trim(CSV_HEADER)) != 0) {
        printf("The first line on the file must be the CSV header.\n");
        return 1;
    }

    strcpy(new_lines[0], CSV_HEADER);

    for (size_t i = 1; i < total; i++) {
        char *line = lines[i];
        if (!line) return 1;

        char line_copy[strlen(line) + 1];
        strcpy(line_copy, line);

        char *line_id = strtok(line_copy, ",");

        if (strcmp(line_id, id) == 0) {
            Task task = task_from_line(line);
            modifier(&task);

            if (task.id == -1) {
                new_lines[i][0] = '\0';
            } else {
                task_to_line(task, new_lines[i]);
            }
        } else {
            snprintf(new_lines[i], CSV_LINE_SIZE, "%s", line);
        }
    }

    FILE *f = csv_open("tasks.csv", "w");
    int result = csv_write_lines(f, new_lines, total);

    if (result != 0) fprintf(stderr, "There was an error while writing lines to the file");

    fclose(f);

    return result;
}

int rm(char *file_name) {
    if (remove("tasks.csv") == 0) {
        printf("File removed successfully.\n");
        return 0;
    } else {
        if (access(file_name, F_OK) != 0) {
            printf("File doesn't exist.\n");
            return 0;
        }

        fprintf(stderr, "Could not remove file.\n");
        return 1;
    }
}
