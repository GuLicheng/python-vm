# Simple function without params 
def foo():
    return

if foo() is None:
    print(1)


# Function with simple params
def add(a, b):
    return a + b

c = add(1, 0)
print(c)

# Function with default params
def add2(a, b = 0):
    return a + b

c = add2(1)
print(c)

# Native function
msg = "Hello World"
c = len(msg)
print(c - 10)

# Native class method
print(msg.upper())

# Closure
def closure():
    x = 1

    def say():
        print(x)

    return say

f = closure()()
