#include <stdio.h>
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

int csv_write(const char *file_name, char *content) {
    FILE *f_read = fopen(file_name, "r");

    if (f_read == NULL) {
        if (csv_create(file_name) != 0) {
            return CSV_ERROR;
        }
    }

    FILE *f = fopen(file_name, "a");

    if (f == NULL) {
        perror("Can't open file even after creating it");
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
