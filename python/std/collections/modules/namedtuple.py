from collections import namedtuple
from typing import NamedTuple

class Fruit(NamedTuple):
    name: str
    weight: float
    varieties: list[str]

def main():
    print(
        "===============================================================================\n"
        "> defaultdict is a dict that creates default values for inexistent keys.\n"
        "> The NamedTuple class provides typing for namedtuples.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.defaultdict \n"
        ">       https://docs.python.org/3.14/library/typing.html#typing.NamedTuple \n"
        "===============================================================================\n"
    )

    Worker = namedtuple("Worker", ["name", "age", "position"])
    print("Worker fields:", Worker._fields)

    w1 = Worker("Joe", "21", "Frontend")
    print("Worker:", w1)
    print("Worker as dict:", w1._asdict())

    w1 = w1._replace(age="23", position="Backend")
    print("Worker after 2 years:", w1)

    w2 = ["Anna", "24", "DevOps"]
    print("New worker came in!", Worker._make(w2))

    print()
    print("NamedTuple from class:", Fruit("apple", 0.5, ["green", "yellow", "red"]))
