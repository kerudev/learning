#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csv.h"
#include "utils.h"

int csv_create(const char *file_name) {
    FILE *f = fopen(file_name, "w");

    if (f == NULL) {
        fprintf(stderr, "Can't create the file\n");
        return CSV_ERROR;
    }

    if (fputs(CSV_HEADER, f) == EOF) {
        fprintf(stderr, "Can't open the file\n");
        fclose(f);

        return CSV_ERROR;
    }

    fclose(f);

    return 0;
}

int csv_check(const char *file_name) {
    FILE *f = fopen(file_name, "r");

    if (f == NULL) {
        if (csv_create(file_name) != 0) {
            return CSV_ERROR;
        }
    }

    return 0;
}

FILE *csv_open(const char *file_name, char *mode) {
    if (csv_check(file_name) != 0) {
        fprintf(stderr, "Can't check the file's existence\n");
        return NULL;
    }

    FILE *f = fopen(file_name, mode);

    if (f == NULL) {
        fprintf(stderr, "Can't open the file even after creating it\n");
        return NULL;
    }

    return f;
}

int csv_write_line(FILE *f, char *line) {
    if (fputs(line, f) == EOF) {
        fprintf(stderr, "Can't write into the file\n");
        fclose(f);
        return CSV_ERROR;
    }

    return 0;
}

int csv_write_lines(FILE *f, char (*lines)[CSV_LINE_SIZE], size_t length) {
    for (size_t i = 0; i < length; i++) {
        char *line = trim(lines[i]);
        if (strcmp(line, "") == 0) continue;
    
        char buffer[CSV_LINE_SIZE];
        snprintf(buffer, sizeof(buffer), "%s\r\n", line);

        int result = csv_write_line(f, buffer);

        if (result != 0) {
            printf("failed on line %ld (%s)\n", i, line);
            return result;
        }
    }

    return 0;
}

char **csv_read_lines(const char *file_name, size_t *total) {
    if (csv_check(file_name) != 0) {
        fprintf(stderr, "Can't check the file's existence\n");
        return NULL;
    }

    FILE *f = fopen(file_name, "r");

    if (f == NULL) {
        fprintf(stderr, "Can't read the file\n");
        return NULL;
    }

    char **lines = NULL;
    size_t capacity = 0;
    size_t count = 0;

    char line[CSV_LINE_SIZE];

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        if (count >= capacity) {
            capacity = capacity == 0 ? 8 : capacity * 2;

            char **tmp = realloc(lines, capacity * sizeof(char *));

            if (!tmp) {
                fprintf(stderr, "Memory allocation failed\n");
                fclose(f);

                for (size_t i = 0; i < count; i++) free(lines[i]);
                free(lines);

                return NULL;
            }

            lines = tmp;
        }

        lines[count] = malloc(strlen(line) + 1);
        strcpy(lines[count], trim(line));

        if (!lines[count]) {
            fprintf(stderr, "Memory allocation failed for line\n");
            fclose(f);

            for (size_t i = 0; i < count; i++) free(lines[i]);
            free(lines);

            return NULL;
        }

        count++;
    }

    fclose(f);

    if (total) *total = count;

    return lines;
}
