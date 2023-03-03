class Exception(object):
    
    def __init__(self, info):
        self.info = info

# def foo():
#     try:
#         print "hello"
#         return
#         print "hi"
#     finally:
#         print "world"


# foo()



def foo(a):
    b = a - 1
    bar(a, b)


def bar(a, b):
    raise Exception("something wrong")


foo(1)

