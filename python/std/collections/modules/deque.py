from collections import deque

class Stack:
    def __init__(self, data: list):
        self.data = deque(data)

    def __repr__(self) -> str:
        return "Stack(%s)" % list(self.data)

    def __iter__(self):
        return reversed(self.data)

    def __len__(self):
        return len(self.data)

    def push(self, item, /):
        self.data.append(item)

    def pop(self):
        return self.data.pop()

    def extend(self, iterable, /):
        self.data.extend(iterable)

    def peek(self):
        return self.data[0] if self.data else None


class Queue:
    def __init__(self, data: list):
        self.data = deque(data)

    def __repr__(self) -> str:
        return "Queue(%s)" % list(self.data)

    def __iter__(self):
        return iter(self.data)

    def __len__(self):
        return len(self.data)

    def enqueue(self, item, /):
        self.data.appendleft(item)

    def dequeue(self):
        return self.data.pop()

    def extend(self, iterable, /):
        self.data.extendleft(iterable)

    def peek(self):
        return self.data[-1] if self.data else None


def main():
    print(
        "===============================================================================\n"
        "> Deques are a generalized class for stacks and queues. They are lists\n"
        "> designed for speed on appends and pops on either end, but they also\n"
        "> support inserting elements at any index.\n"
        ">\n"
        "> This example implements Stack and Queue classes using a deque.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.deque \n"
        "> Stack: https://en.wikipedia.org/wiki/Stack_(abstract_data_type) \n"
        "> Queue: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) \n"
        "===============================================================================\n"
    )

    data = ["cherry", "tangerine", "banana"]

    print(f"Data sample: {data}\n")

    print("stack: initializing")
    stack = Stack(data)
    print(stack)

    print("stack: appending more fruits")
    stack.push("apple")
    stack.push("blueberry")
    stack.push("grape")
    print(stack)

    print("stack first  pop:", stack.pop())
    print("stack second pop:", stack.pop())
    print("stack third  pop:", stack.pop())

    print("stack iteration")
    n = len(stack)
    for idx, element in enumerate(stack):
        print(n - idx - 1, element)

    print("stack peek:", stack.peek())

    print()
    print("Initializing queue")
    queue = Queue(data)
    print(queue)

    print("queue: appending more fruits")
    queue.enqueue("apple")
    queue.enqueue("blueberry")
    queue.enqueue("grape")
    print(queue)

    print("queue first  pop:", queue.dequeue())
    print("queue second pop:", queue.dequeue())
    print("queue third  pop:", queue.dequeue())

    print("queue iteration")
    n = len(queue)
    for idx, element in enumerate(queue):
        print(n - idx - 1, element)

    print("queue peek:", queue.peek())
