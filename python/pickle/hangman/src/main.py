import os
from game import Game

def main():
    try:
        if os.path.exists("state.pkl"):
            print("Loading previous game...")
            game = Game.load()
        else:
            print("New game...")
            game = Game()

        game.run()

    except KeyboardInterrupt:
        game.save()

if __name__ == "__main__":
    main()
