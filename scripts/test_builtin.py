# Exception

e = Exception(1, 2, 3)
print(e.args)

e = StopIteration()
print(e.args)

ls = [1, 2, 3]


iterator = iter(ls)

try:
    val = next(iterator)
    print(val)
    val = next(iterator)
    print(val)
    val = next(iterator)
    print(val)
    val = next(iterator)
    print(val)
except StopIteration:
    print("Over")