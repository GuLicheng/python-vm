class A(object):

    number = 1

    def __init__(self, v):
        self.value = v

print("class")
a = A(2)
print(a.number)
print(a.value)