#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"

char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char* back;
    int len = strlen(s);

    if (len == 0) return(s);

    back = s + len - 1;

    while (back >= s && isspace((unsigned char)*back)) back--;

    *(back+1) = '\0';
    return s;
}

char *trim(char *s) {
    char *copy = malloc(strlen(s) + 1);

    if (!copy) return NULL;
    strcpy(copy, s);

    return ltrim(rtrim(copy));
}
