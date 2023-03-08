

def sequence(max):

    i = 3
    while i < max:

        yield i
        i = i + 1


for val in sequence(10):
    print(val)
