#include <stdio.h>
#include <string.h>

int main() {
    char name[30];
    int age;
    
    printf("Type your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Type your age: ");
    scanf("%d", &age);

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
