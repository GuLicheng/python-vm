
class Integer(object):

    def __init__(self, value):
        self.value = value

    def __add__(self, other):
        return Integer(self.value + other.value)
    

a = Integer(1)
b = Integer(2)

c = a + b

print(c.value)
