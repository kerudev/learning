import argparse
from argparse import Namespace
from enum import StrEnum


class Action(StrEnum):
    PLAY = "play"
    SCORE = "score"
    ADD = "add"


def parse() -> Namespace:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser(Action.PLAY.value, help="play the game")

    score = subparsers.add_parser(Action.SCORE.value, help="manages your score")
    score_group = score.add_mutually_exclusive_group(required=True)
    score_group.add_argument("--show", action="store_true", help="see your score")
    score_group.add_argument("--reset", action="store_true", help="resets the stored score")

    add = subparsers.add_parser(Action.ADD.value, help="add a word to the game's database")
    add.add_argument("word", help="the word that will be added")

    return parser.parse_args()
