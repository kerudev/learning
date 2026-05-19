from collections import defaultdict

def main():
    print(
        "===============================================================================\n"
        "> defaultdict is a dict that creates default values for non-existent keys.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.defaultdict \n"
        "===============================================================================\n"
    )

    dictionary = {"a": 4, "b": 2}
    dictionary_default = defaultdict(dict)

    for k, v in dictionary.items():
        dictionary_default.setdefault(k, []).append(v)

    print("dict:", dictionary)
    print("defaultdict with dict:", dictionary_default.items())

    print()

    string = "The quick brown fox jumps over the lazy dog"
    string_default = defaultdict(int)

    for char in string:
        string_default[char] += 1

    print("str:", string)
    print("defaultdict with int:", string_default.items())

    print()

    foods = [
        ("fruit", "apple"),
        ("fruit", "pear"),
        ("fruit", "orange"),
        ("vegetable", "lettuce"),
        ("vegetable", "cucumber"),
    ]
    foods_default = defaultdict(list)

    for group, food in foods:
        foods_default[group].append(food)

    print("list:", foods)
    print("defaultdict from list:", foods_default.items())
