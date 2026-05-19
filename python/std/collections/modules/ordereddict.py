from collections import OrderedDict

class Cache:
    def __init__(self, data):
        self.data = OrderedDict(data)

    def __repr__(self) -> str:
        return "Cache(%s)" % self.data

    def __getitem__(self, key):
        self.data.move_to_end(key, last=False)
        return self.data[key]

def main():
    print(
        "===============================================================================\n"
        "> OrderedDict is a dict that remembers the order in which keys are inserted.\n"
        "> It was designed for reordering operations rather than space efficiency and\n"
        "> iteration speed.\n"
        "> Before Python 3.7, dictionaries did not preserve insertion order.\n"
        ">\n"
        "> This example implements a Cache class that moves items on top when accessed.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.OrderedDict \n"
        "===============================================================================\n"
    )

    data = {
        "a": 0,
        "b": 1,
        "c": 2,
        "d": 3,
        "e": 4,
        "f": 5,
        "g": 6,
        "h": 7,
        "i": 8,
        "j": 9,
    }

    cache = Cache(data)

    print("Initial cache:", cache)

    cache["h"]
    cache["e"]
    cache["j"]

    print("Final cache:", cache)
