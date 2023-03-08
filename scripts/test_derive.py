class Z(object):
    def say(self):
        print("I am Z")

class X(Z):
    def say(self):
        print("I am X")

class Y(Z):
    def say(self):
        print("I am Y")       

class A(X, Y):
    def say(self):
        print("I am A")

class B(Z):
    def say(self):
        print("I am B")

class C(B, A):
    pass

c = C()
c.say()


class F1(object): pass

class F2(object): pass

class F3(F1, F2): pass

class E(F3, F1): pass




