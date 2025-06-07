#ifndef CSV_H
#define CSV_H

#define CSV_ERROR 1
#define CSV_HEADER "id,content,checked\n"
#define CSV_LINE_SIZE 128

int csv_create(const char *file_name);
int csv_check(const char *file_name);

FILE *csv_open(const char *file_name, char *mode);

int csv_write_line(FILE *f, char *line);
int csv_write_lines(FILE *f, char (*lines)[CSV_LINE_SIZE], size_t length);
char **csv_read_lines(const char *file_name, size_t *total);

#endif
