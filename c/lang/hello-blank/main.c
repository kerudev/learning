#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 30

int is_number(const char *s) {
    if (*s == '\0') return 0;
    
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }

    return 1;
}

int main() {
    char name[SIZE];
    char age_str[SIZE];
    unsigned int age;
    
    printf("Type your name: ");
    fgets(name, sizeof(name), stdin);

    if (strchr(name, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        name[strcspn(name, "\n")] = 0;
    }

    printf("Type your age: ");
    fgets(age_str, sizeof(age_str), stdin);
    age_str[strcspn(age_str, "\n")] = '\0';

    if (!is_number(age_str)) {
        printf("Invalid input: please enter only positive numbers.\n");
        return 1;
    }

    age = (unsigned int) strtoul(age_str, NULL, 10);

    if (age > 130) {
        printf("Invalid input for age. Are you sure it's is correct?\n");
        return 1;
    }

    printf("You are %s, age %d\n", name, age);

    int count = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] != ' ') {
            count++;
        }
    }

    printf("Your name is %d characters long\n", count);

    return 0;
}
