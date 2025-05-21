#include <stdio.h> 
#include "task.h"

typedef struct Todo {
    Task *tasks;
    size_t size;
    size_t capacity;
} Todo;
