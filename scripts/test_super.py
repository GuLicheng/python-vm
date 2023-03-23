# Inherit from usr-defined class
class Base:

    def __init__(self):
        self.value = 1


class Derived(Base):

    def __init__(self):
        super(Derived, self).__init__()


d = Derived()
print(d.value)

# Inherit from builtin class
class MyException(Exception):

    def __init__(self, msg, ec):
        super(MyException, self).__init__(msg, ec)


e = MyException("Error", 0)
print(e.args)
