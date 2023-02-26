
class Number(object):

    def __init__(self, value):
        self.value = value

    def __add__(self, other):
        print("call __add__")
        return Number(self.value + other.value)
    
    def __sub__(self, other):
        print("call __sub__")
        return Number(self.value - other.value)
    
    def __mul__(self, other):
        print("call __mul__")
        return Number(self.value * other.value)
    
    def __le__(self, other):
        print("call __le__")
        return self.value < other.value
    
    # def __truediv__(self, other):
    #     return Number(self.value / other.value)
    
    # def __floordiv__(self, other):
    #     return Number(self.value // other.value)

    def __hash__(self):
        pass
    
    def __str__(self):
        pass

    def __deepcopy__(self):
        pass

a = Number(1)
b = Number(2)

c = a + b
print(c.value)

c = a - b 
print(c.value)

c = a * b
print(c.value)

print(a < b)
