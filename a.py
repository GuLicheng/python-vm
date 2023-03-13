
from typing import Iterable, Iterator

print(issubclass(Iterator, Iterable))
print(issubclass(Iterable, Iterator))

def seq():
    yield from [1, 2, 3]

for val in seq():
    print(val)
