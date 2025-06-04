import os
import pickle
from .sqlite import Sqlite, ScoreField


class Game:
    def __init__(self):
        self.__db = Sqlite()
        self.__word = self.__db.word()
        self.__tries = len(self.__word) + 2
        self.__state = len(self.__word) * ["_"]
        self.__left = [chr(i) for i in range(ord("a"), ord("z") + 1)]
        self.__used = []

    def __getstate__(self):
        state = self.__dict__.copy()
        del state["_Game__db"]
        return state

    def __setstate__(self, state):
        self.__dict__.update(state)
        self.__db = Sqlite()

    @staticmethod
    def play():
        if os.path.exists("state.pkl"):
            print("Loading previous game...")
            game = Game.load()
        else:
            print("New game...")
            game = Game()

        try:
            game.run()
        except KeyboardInterrupt:
            game.save()

    @staticmethod
    def load() -> "Game":
        with open("state.pkl", "rb") as f:
            return pickle.load(f)

    def save(self):
        with open("state.pkl", "wb") as f:
            pickle.dump(self, f)

    def run(self):
        while True:
            os.system("clear")

            print("  Tries left:", self.__tries)
            print("Letters left:", ", ".join(self.__left))
            print("Letters used:", ", ".join(self.__used))
            print()
            print(" ".join(self.__state))

            if self.__tries < 1:
                self.lose()
                break

            if "".join(self.__state) == self.__word:
                self.win()
                break

            char = input("> ").lower().strip()
            print()

            if char not in self.__left:
                print("Invalid input.")
                continue

            self.__left.remove(char)
            self.__used.append(char)

            if char not in self.__word:
                self.__tries -= 1
                continue

            for index, letter in enumerate(self.__word):
                if char == letter:
                    self.__state[index] = char

    def win(self):
        print(f"> You win! The word was {self.__word}")
        self.__db.update_score(ScoreField.WINS, self.__word)

        if os.path.exists("state.pkl"):
            os.remove("state.pkl")

    def lose(self):
        print(f"> You lose! The word was {self.__word}")
        self.__db.update_score(ScoreField.LOSES, self.__word)
