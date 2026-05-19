from collections import ChainMap

def main():
    print(
        "===============================================================================\n"
        "> ChainMap creates a dict-like structure that stores references to dicts.\n"
        "> It does not overwrite data, just accesses it from references.\n"
        "> Read priority from left to right.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.ChainMap \n"
        "===============================================================================\n"
    )

    default = {"host": "localhost", "port": 8000}
    env = {"port": 9000}
    cli = {"debug": True}

    print("default:", default)
    print("env:", env)
    print("cli:", cli)

    print()

    chain = ChainMap(env, default)
    print("chain:", chain)

    print("chain.maps:", chain.maps)

    chain = chain.new_child(cli)
    print("chain.new_child(cli):", chain)

    print()

    print('chain["default"]:', default)
    print('chain["env"]:', env)
    print('chain["cli"]:', cli)
