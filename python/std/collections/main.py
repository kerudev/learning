import argparse
from enum import Enum
import importlib.util

class Collection(str, Enum):
    NAMEDTUPLE = "namedtuple"
    DEQUE = "deque"
    CHAINMAP = "chainmap"
    COUNTER = "counter"
    ORDEREDDICT = "ordereddict"
    DEFAULTDICT = "defaultdict"
    USERDICT = "userdict"
    USERLIST = "userlist"
    USERSTRING = "userstring"


def parser():
    parser = argparse.ArgumentParser()
    collection = parser.add_subparsers(dest="collection", required=True)

    for variant in Collection:
        collection.add_parser(variant.value, help=f"Run the '{variant.value}' example")

    return parser.parse_args()


if __name__ == "__main__":
    args = parser()
    collection = args.collection

    for variant in Collection:
        if collection == variant:
            spec = importlib.util.spec_from_file_location(collection, f"modules/{collection}.py")
            example = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(example)

            example.main()
