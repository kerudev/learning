#!/usr/bin/python3

from modules.args import parse, Action
from modules.game import Game
from modules.sqlite import Sqlite


def main():
    args = parse()

    match args.command:
        case Action.PLAY:
            Game.play()

        case Action.SCORE:
            if args.show:
                Sqlite().print_score()
            if args.reset:
                Sqlite().reset_score()

        case Action.ADD:
            Sqlite().add(args.word)


if __name__ == "__main__":
    main()
