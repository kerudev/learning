#include <stdio.h>
#include <string.h>
#include "game.h"

int get_result(Move player, Move cpu) {
    int result = (3 + move_to_int(player) - move_to_int(cpu)) % 3;

    if (result == 0) {
        printf("tie!\n");
    } else if (result == 1) {
        printf("%s beats %s! You win!\n", move_to_string(player), move_to_string(cpu));
    } else if (result == 2) {
        printf("%s beats %s! You lose!\n", move_to_string(player), move_to_string(cpu));
    }

    return result < 2 ? 0 : 1; 
}