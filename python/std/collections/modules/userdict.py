from __future__ import annotations

from collections import UserDict

class Bidict(UserDict):
    def _reverse(self):
        return {v: k for k, v in self.data.items()}

    @property
    def inverse(self) -> Bidict:
        return Bidict(self._reverse())

def main():
    print(
        "===============================================================================\n"
        "> UserDict is a wrapper around 'dict' to create dict-like subclasses.\n"
        "> It's not that useful as one can already make a subclass of 'dict', but it\n"
        "> already does some of the work.\n"
        ">\n"
        "> This example implements a simple Bidict, which is a dictionary that allows\n"
        "> getting data by key or value.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.UserDict \n"
        "> Bidict: https://github.com/jab/bidict/tree/main \n"
        "===============================================================================\n"
    )
    
    data = {
        "cherry": "red",
        "tangerine": "orange",
        "banana": "yellow",
        "apple": "green",
        "blueberry": "blue",
        "grape": "purple",
    }

    fruit = Bidict(data)
    print("Bidict :", fruit)
    print('- Getting a value:', fruit["cherry"])
    print()

    color = fruit.inverse
    print("Inverse:", color)
    print('- Getting a value from the inverse:', color["purple"])
