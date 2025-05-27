#ifndef CSV_H
#define CSV_H

#define CSV_ERROR 1
#define CSV_HEADER "id,content,checked"

typedef struct csv csv;

int csv_create(const char *file_name);

int csv_write(const char *file_name, char *content);

#endif
