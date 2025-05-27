#include <stdio.h>
#include <string.h>

#define SIZE 30

int main() {
    char name[SIZE];
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
    scanf("%d", &age);

    if (age < 0) {
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
