import os
import platform
import py_compile

SRC = "scripts"
DST = "test"

def build():

    assert "2.7" <= platform.python_version() < "3.0"

    for file in os.listdir(SRC):
        src = os.path.join(SRC, file)
        dst = os.path.join(DST, file + 'c')
        py_compile.compile(file=src, cfile=dst)

    print("OK")

if __name__ == "__main__":

    build()
    

