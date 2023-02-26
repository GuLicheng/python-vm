
class Number(object):

    def __init__(self, value):
        self.value = value

    def __add__(self, other):
        return Number(self.value + other.value)
    
    def __sub__(self, other):
        return Number(self.value - other.value)
    
    def __mul__(self, other):
        return Number(self.value * other.value)
    
    # def __truediv__(self, other):
    #     return Number(self.value / other.value)
    
    # def __floordiv__(self, other):
    #     return Number(self.value // other.value)
    

a = Number(1)
b = Number(2)

c = a + b
print(c.value)

c = a - b 
print(c.value)

c = a * b
print(c.value)