from argparse import Namespace
import sqlite3
import random
from .utils import get_random_words
from enum import Enum


class ScoreField(str, Enum):
    WINS = "wins"
    LOSES = "loses"


class Sqlite:
    def __init__(self):
        self._connection = sqlite3.connect("words.sqlite")
        self._cursor = self._connection.cursor()

        self._cursor.execute("""
            CREATE TABLE IF NOT EXISTS words (
                id      INTEGER PRIMARY KEY AUTOINCREMENT,
                word    TEXT UNIQUE NOT NULL,
                wins    INTEGER NOT NULL,
                loses   INTEGER NOT NULL
            )
        """)

        self._connection.commit()

    def __insert_many(self, parameters: list[tuple[str]]):
        self._cursor.executemany("""
            INSERT INTO words (word, loses, wins)
            VALUES(?, 0, 0)
        """, parameters)

        self._connection.commit()

    def __populate(self):
        words = [get_random_words(length=n, total=10) for n in range(4, 10)]
        words = set().union(*words)

        parameters = [(word,) for word in words]

        self.__insert_many(parameters)

    def add_word(self, word: str):
        self._cursor.execute(f"""
            INSERT INTO words (word, loses, wins)
            VALUES('{word}', 0, 0)
        """)

        self._connection.commit()

    def add_random(self, args: Namespace):
        length = args.length or random.randint(4, 10)

        words = get_random_words(length=length, total=args.number)
        parameters = [(word,) for word in words]

        print(parameters)

        self.__insert_many(parameters)

    def count(self) -> int:
        result = self._cursor.execute("SELECT COUNT(*) AS count FROM words")
        return result.fetchone()[0]

    def get_word(self) -> str:
        count = self.count()

        if not count:
            self.__populate()
            count = self.count()

        word_id = random.randint(1, count)

        result = self._cursor.execute(f"SELECT word FROM words WHERE id = {word_id}")

        return result.fetchone()[0]

    def update_score(self, field: ScoreField, word: str):
        self._cursor.execute(f"UPDATE words SET {field.value} = {field.value} + 1 WHERE word = '{word}'")
        self._connection.commit()

    def reset_score(self):
        query = [
            "UPDATE words",
            f"SET {ScoreField.WINS.value} = 0,",
            f"    {ScoreField.LOSES.value} = 0",
            "WHERE id > 0",
        ]

        self._cursor.execute(" ".join(query))
        self._connection.commit()

        print("Score resetted.")

    def print_score(self):
        result = self._cursor.execute("SELECT * FROM words WHERE wins > 0 OR loses > 0")

        words = result.fetchall()
        columns = ["id", "word", "wins", "loses"]

        widths = [max(len(str(row[i])) for row in [columns] + words) for i in range(len(columns))]

        col_format = " | ".join(f"{{:<{width}}}" for width in widths)
        separator = "-+-".join("-" * width for width in widths)

        print(col_format.format(*columns))
        print(separator)
        for word in words:
            print(col_format.format(*word))
