import math

from collections import UserList

class Bisect(UserList):
    def __init__(self, initlist):
        super().__init__(initlist)

        self.l = 0
        self.r = len(self.data)

        self._stop = False
        self._root = None

    @property
    def _mid(self):
        return (self.r + self.l) // 2
    
    @property
    def _steps(self):
        n = len(self.slice())

        if n <= 1:
            return 0

        return math.floor(math.log2(n))

    def run(self):
        self.start()

        choices = ["good", "bad"]

        while not self._stop:
            slice = self.slice()
            len_slice = len(slice)
            ptr = self.pointer()

            print(f"There are {len_slice} revisions left (roughly {self._steps} steps)")
            print("slice:", " ".join(slice))
            print("ptr  :", ptr)

            choice = input("> ")

            while choice not in choices:
                print("Please write one of the following:", choices)
                choice = input("> ")

            print(f"Marked {ptr} as {choice}\n")

            func = getattr(self, choice)
            func()

            if len_slice == 1:
                self.stop()

        print("The bad commit was", self.result())

    def result(self):
        return self._root

    def start(self):
        self._stop = False

    def good(self):
        self.r = self.data.index(self.pointer())

    def bad(self):
        self.l = self.data.index(self.pointer())

    def stop(self):
        self._stop = True
        self._root = self.pointer()

    def pointer(self):
        return self.data[self._mid]

    def slice(self):
        return self.data[self.l:self.r]

def main():
    print(
        "===============================================================================\n"
        "> UserList is a wrapper around 'list' to create list-like subclasses.\n"
        "> It's not that useful as one can already make a subclass of 'list', but it\n"
        "> already does some of the work.\n"
        ">\n"
        "> This example implements a Bisect class, which is a root-finding method that\n"
        "> Git uses in 'git bisect' to find a bad commit.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.UserList \n"
        "> git bisect: https://git-scm.com/docs/git-bisect \n"
        "===============================================================================\n"
    )

    commits = [
        "commit 0",
        "commit 1",
        "commit 2",
        "commit 3",
        "commit 4",
        "commit 5",
        "commit 6",
        "commit 7",
        "commit 8",
        "commit 9",
    ]

    bisect = Bisect(commits)
    bisect.run()
