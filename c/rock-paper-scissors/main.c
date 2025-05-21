#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    char choice[10];

    printf("Rock, paper, scissors: ");
    scanf("%s", choice);

    srand(time(NULL));

    int rnd = rand();
    int num = rnd % 3;

    char result[9];

    switch (num) {
        case 0:
            strcpy(result, "rock");
            break;

        case 1:
            strcpy(result, "paper");
            break;

        case 2:
            strcpy(result, "scissors");
            break;
    }

    printf("%s vs %s\n", choice, result);

    if (strcmp(choice, result) == 0) {
        printf("tie!\n");
        return 0;
    }
    
    if (strcmp(choice, "rock") == 0) {
        if (strcmp(result, "paper") == 0) {
            printf("paper beats rock! you lose!\n");
            return 1;
        } else if (strcmp(result, "scissors") == 0) {
            printf("rock beats scissors! you win!\n");
            return 0;
        }
    } else if (strcmp(choice, "paper") == 0) {
        if (strcmp(result, "rock") == 0) {
            printf("paper beats rock! you win!\n");
            return 0;
        }
        else if (strcmp(result, "scissors") == 0) {
            printf("scissors beats paper! you lose!\n");
            return 1;
        }
    } else if (strcmp(choice, "scissors") == 0) {
        if (strcmp(result, "paper") == 0) {
            printf("scissors beats paper! you win!\n");
            return 0;
        }
        else if (strcmp(choice, "scissors") == 0 && strcmp(result, "rock") == 0) {
            printf("rock beats scissors! you lose!\n");
            return 1;
        }
    }

    printf("You didn't enter a correct value\n");

    return 1;
}
