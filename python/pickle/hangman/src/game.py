from sqlite import Sqlite

class Game:
    def __init__(self):
        self.__state = None
        self.__word = Sqlite().word()

    def run(self):
        pass

    def save(self):
        pass
