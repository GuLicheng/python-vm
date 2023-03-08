
# def foo():
#     try:
#         print "hello"
#         return
#         print "hi"
#     finally:
#         print "world"


# foo()

class Exception(object): pass
    

def foo(a):
    b = a - 1
    bar(a, b)


def bar(a, b):
    # raise Exception("something wrong")
    raise 0

try:
    foo(1)
except Exception:
    print("E1")
except int:
    print("E2")

