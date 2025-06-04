import argparse
from argparse import Namespace
from enum import StrEnum


class Action(StrEnum):
    PLAY = "play"
    SCORE = "score"
    ADD = "add"
    ADD_WORD = "word"
    ADD_RANDOM = "random"


def parse() -> Namespace:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser(Action.PLAY.value, help="play the game")

    score = subparsers.add_parser(Action.SCORE.value, help="manages your score")
    score_group = score.add_mutually_exclusive_group(required=True)

    score_group.add_argument("--show", action="store_true", help="see your score")
    score_group.add_argument("--reset", action="store_true", help="resets the stored score")

    add = subparsers.add_parser(Action.ADD.value, help="add a word to the game's database")
    add_subparsers = add.add_subparsers(dest="add_type", required=True)

    word_parser = add_subparsers.add_parser(Action.ADD_WORD.value, help="add a specific word")
    word_parser.add_argument("word", help="The word to add")

    random_parser = add_subparsers.add_parser(Action.ADD_RANDOM.value, help="add random words")
    random_parser.add_argument("--length", type=int, help="length of each word")
    random_parser.add_argument("--number", type=int, default=1, help="how many random words to add")

    return parser.parse_args()
