import os
import platform
import py_compile

SRC = "scripts"
DST = "test"

def build():

    assert platform.version() < "3.0"

    for file in os.listdir(SRC):
        src = os.path.join(SRC, file)
        dst = os.path.join(DST, file + 'c')
        py_compile.compile(file=src, cfile=dst)

    print("OK")

if __name__ == "__main__":

    build()
    

"""
6400008400005a00006500008300006401006b080072200065020047486e000064010053
6400008400005a00006500008300006401006b080072200065020047486e000064010053

0x000001d4c5f871b0
0x000001d4c5f86ef0


0x000001d4c5f871b0
"""