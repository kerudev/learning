typedef enum {
    ROCK,
    PAPER,
    SCISSORS,
    ERROR,
} Move;

Move int_to_move(int num);
Move string_to_move(char* choice);
const char *move_to_string(Move move);
int move_to_int(Move move);
