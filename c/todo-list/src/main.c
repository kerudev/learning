#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 

#include "task.h"

int main(int argc, char const *argv[]) {
    FILE *f = fopen("tasks.csv", "a+");

    if (f == NULL) {
        perror("Can't open the file");
        return 1;
    }

    Task task = new_task("abcde");

    char str[50];
    sprintf(str, "%d,%s,%d\n", task.id, task.content, task.checked);

    fputs(str, f);
    fclose(f);

    return 0;
}
