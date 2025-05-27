#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "game.h"

#define SIZE 10

void sanitize_input(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }

    for (size_t i = 0; s[i]; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

int main() {
    srand(time(NULL) ^ getpid());

    char player[SIZE];

    printf("rock, paper, scissors: ");

    if (fgets(player, sizeof(player), stdin) == NULL) {
        fprintf(stderr, "Invalid move.\n");
        return 1;
    }

    sanitize_input(player);

    Move player_move = string_to_move(player);

    if (player_move == ERROR) {
        fprintf(stderr, "Invalid move.\n");
        return 1;
    }

    int cpu = rand() % 3;
    Move cpu_move = int_to_move(cpu);

    printf("%s vs %s\n", player, move_to_string(cpu_move));

    return get_result(player_move, cpu_move);
}
