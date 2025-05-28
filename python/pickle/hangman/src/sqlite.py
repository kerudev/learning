import sqlite3
import random

class Sqlite:
    def __init__(self):
        self.__connection = sqlite3.connect("game.sqlite")
        self.__cursor = self.__connection.cursor()

        self.__cursor.execute("""
            CREATE TABLE IF NOT EXISTS words (
                id      INTEGER PRIMARY KEY AUTOINCREMENT,
                word    TEXT NOT NULL,
                tries   INTEGER NOT NULL,
                wins    INTEGER NOT NULL
            )
        """)

        self.__connection.commit()

    def __populate(self):
        words = [
            "curriculum", "angle", "witness", "committee", "staircase",
            "junior", "departure", "reinforce", "essential", "mine",
            "vein", "unlawful", "thirsty", "bell", "refer",
            "sea", "country", "shape", "environmental", "consciousness",
            "cutting", "zone", "riot", "salvation", "helpless",
            "spot", "attic", "other", "hospitality", "listen",
            "manner", "am", "lily", "slice", "economist", "satisfied",
            "thrust", "wire", "testify", "literacy", "carriage",
            "pierce", "sweet", "disturbance", "opposed",
            "boy", "commitment", "import", "civilian", "guest"
        ]

        parameters = [(word,) for word in words]

        self.__cursor.executemany("""
            INSERT INTO words (word, tries, wins)
            VALUES(?, 0, 0)
        """, parameters)

        self.__connection.commit()

    def count(self) -> int:
        result = self.__cursor.execute("SELECT COUNT(*) AS count FROM words")
        return result.fetchone()[0]

    def word(self) -> str:
        count = self.count()

        if not count:
            self.__populate()
            count = self.count()

        word_id = random.randint(1, count + 1)

        self.__cursor.execute(f"SELECT * FROM words WHERE id = {word_id}")
