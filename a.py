class F1(object): pass

class F2(object): pass

class F3(F1, F2): pass

class E(F3, F1): pass

# 打印C类的调用路径顺序(注意要类名.__mro__)
print(E.__mro__)

print(tuple(reversed(E.__mro__)))