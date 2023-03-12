sequence = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

def add_one(x):
    return x + 1

def is_less_than_5(x):
    return x < 5

# filter  -> [0, 1, 2, 3, 4]
# add_one -> [1, 2, 3, 4, 5]
# take    -> [1, 2, 3]
# drop    -> [2, 3]

for val in drop(1, take(3, map(add_one, filter(is_less_than_5, sequence)))):
    print(val)
