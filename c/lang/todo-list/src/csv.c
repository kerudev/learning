#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csv.h"

int csv_create(const char *file_name) {
    FILE *f = fopen(file_name, "w");

    if (f == NULL) {
        perror("Can't create the file");
        return CSV_ERROR;
    }

    if (fputs(CSV_HEADER, f) == EOF) {
        perror("Can't open the file");
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

int csv_write(const char *file_name, char *content) {
    if (csv_check(file_name) != 0) {
        perror("Can't check the file's existence");
        return CSV_ERROR;
    }

    FILE *f = fopen(file_name, "a");

    if (f == NULL) {
        perror("Can't open the file even after creating it");
        return CSV_ERROR;
    }

    if (fputs(content, f) == EOF) {
        perror("Can't write into the file");
        fclose(f);
        return CSV_ERROR;
    }

    fclose(f);

    return 0;
}

char **csv_read_lines(const char *file_name, size_t *total) {
    if (csv_check(file_name) != 0) {
        perror("Can't check the file's existence");
        return NULL;
    }

    FILE *f = fopen(file_name, "r");

    if (f == NULL) {
        perror("Can't read the file");
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
                perror("Memory allocation failed");
                fclose(f);

                for (size_t i = 0; i < count; i++) free(lines[i]);
                free(lines);

                return NULL;
            }

            lines = tmp;
        }

        lines[count] = strdup(line);

        if (!lines[count]) {
            perror("Memory allocation failed for line");
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
