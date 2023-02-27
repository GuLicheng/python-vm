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
    
    # relation ship operator
    def __lt__(self, other):
        print("call __lt__")
        return self.value < other.value

    def __gt__(self, other):
        print("call __gt__")
        return self.value > other.value

    def __le__(self, other):
        print("call __le__")
        return self.value <= other.value
    
    def __ge__(self, other):
        print("call __ge__")
        return self.value >= other.value

    # def __truediv__(self, other):
    #     return Number(self.value / other.value)
    
    # def __floordiv__(self, other):
    #     return Number(self.value // other.value)

    def __hash__(self):
        print("call __hash__")
        return hash(self.value)

    def __str__(self):
        pass

    def __deepcopy__(self):
        pass

    def __getitem__(self, key):
        print("call __getitem__")
        if key == "value":
            return self.value
        else:
            return None
        
    def __setitem__(self, key, value):
        print("call __setitem__")
        print([key, value])
        if key == "value":
            self.value = value
        else:
            print("Error")

    def __call__(self, value):
        print("call __call__")
        if value == "value":
            print(self.value)
        else:
            print("Unknown value.")



a = Number(1)
b = Number(2)

c = a + b
print(c.value)

c = a - b 
print(c.value)

c = a * b
print(c.value)

print(a < b)
print(a <= b)
print(a > b)
print(a >= b)

print(hash(a))

print(a["value"])
a["value"] = -1
print(a.value)

a("value")
a("unknown")



