import os
import argparse
from build_scripts import build 

if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    parser.add_argument("--exec", type=str, default="../bin/release/CppPython.exe")
    parser.add_argument("--py_src_dir", type=str, default="scripts")
    parser.add_argument("--py_dst_dir", type=str, default="test")

    args = parser.parse_args()

    build(args.py_src_dir, args.py_dst_dir)

    




