class Base(object):
    def __init__(self):
        self.value = 0

class A(Base):
    def __init__(self):
        super(Base, self).__init__()


import dis
dis.dis(A)
"""
Disassembly of __init__:
  7           0 LOAD_GLOBAL              0 (super)   
              3 CALL_FUNCTION            0
              6 LOAD_ATTR                1 (__init__)
              9 CALL_FUNCTION            0
             12 POP_TOP
             13 LOAD_CONST               0 (None)    
             16 RETURN_VALUE
"""