#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "game.h"

Move int_to_move(int num) {
    switch (num) {
        case 0: return ROCK;
        case 1: return PAPER;
        case 2: return SCISSORS;
    }

    return ERROR;
}

Move string_to_move(char* choice) {
    for (int i = 0; choice[i]; i++) choice[i] = tolower(choice[i]);

    if (strcmp(choice, "rock") == 0) return ROCK;
    if (strcmp(choice, "paper") == 0) return PAPER;
    if (strcmp(choice, "scissors") == 0) return SCISSORS;

    return ERROR;
}

const char *move_to_string(Move move) {
    switch (move) {
        case ROCK: return "rock";
        case PAPER: return "paper";
        case SCISSORS: return "scissors";
        default: return "error";
    }
}

int move_to_int(Move move) {
    switch (move) {
        case ROCK: return 0;
        case PAPER: return 1;
        case SCISSORS: return 2;
        default: return -1;
    }
}
