# python/pickle/hangman

In this project I learned:
- What Pickle is and when to use it.
- How to use Pickle to save and recover the program's state.
- How to use the `__getstate__` and `__setstate__` magic methods.

Run by using `python hangman.py` (or `./hangman.py` if Python is at `/usr/bin/python3`).

Using `./hangman.py -h` displays the following commands:
- `play`: starts a new game

- `score`: manages your score
  - `--show`: displays your score
  - `--show`: resets your score

- `add`: add a word to the game's database
  - `word`: add a specific word
  - `random`: add random words
    - `--length`: length of each word 
    - `--number`: how many random words to add 
