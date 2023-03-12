import os
import platform
import py_compile

SRC = "scripts"
DST = "test"

def build(src = SRC, dst = DST):

    assert "2.7" <= platform.python_version() < "3.0"

    for file in os.listdir(src):
        py_compile.compile(
            file=os.path.join(src, file), 
            cfile=os.path.join(dst, file + 'c')
        )

    print("OK")

if __name__ == "__main__":

    build()
    

