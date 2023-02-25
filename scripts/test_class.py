class Z(object):
    def say(self):
        print("I am Z")

class X(Z):
    def say(self):
        print("I am X")

# class Y(Z):
#     def say(self):
#         print("I am Y")       

# class A(X, Y):
#     def say(self):
#         print("I am A")

# class B(Z):
#     def say(self):
#         print("I am B")

# class C(B, A):
#     pass

c = X()
c.say()
