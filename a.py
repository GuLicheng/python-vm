import os

def adb_shell(cmd):
    exit_code = os.system(cmd)
    return exit_code>>8


adb_shell("cd bin/debug && ./CppPython.exe")

from typing import Iterable