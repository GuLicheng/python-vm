class Exception(object): pass

def range(start, end, step):

    if step == 0:
        raise Exception()

    elif step > 0:

        if start > end:
            raise Exception()

        while start < end:
            yield start
            start = start + step
    else:

        if start < end:
            raise Exception()
        
        while start > end:
            yield start
            start = start + step

for val in range(0, 10, 2):
    print(val)

for val in range(10, 0, -2):
    print(val)