#ifndef CSV_H
#define CSV_H

#define CSV_ERROR 1
#define CSV_HEADER "id,content,checked\n"
#define CSV_LINE_SIZE 128

typedef struct csv csv;

int csv_create(const char *file_name);
int csv_check(const char *file_name);
int csv_write(const char *file_name, char *content);
char **csv_read_lines(const char *file_name, size_t *total);

#endif
