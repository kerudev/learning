from collections import Counter

def main():
    print(
        "===============================================================================\n"
        "> Counter is a simple yet useful class that counts the number of duplicates in\n"
        "> an iterable.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.Counter \n"
        "===============================================================================\n"
    )

    dictionary = {"a": 4, "b": 2}
    dictionary_counter = Counter(dictionary)
    print("dict:", dictionary)
    print("Counter from dict:", dictionary_counter)

    print()

    string = "The quick brown fox jumps over the lazy dog"
    string_counter = Counter(string)
    print("str:", string)
    print("Counter from string:", string_counter)

    print()

    letters = ["c", "d", "a", "a", "b", "d", "b", "b"]
    letters_counter = Counter(letters)
    print("list:", letters)
    print("Counter from list:", letters_counter)

    print("Elements:", sorted(letters_counter.elements()))
    print("Most common:", letters_counter.most_common())
    print("Subtract:", letters_counter.subtract())
    print("Total:", letters_counter.total())
