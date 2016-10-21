# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

import os
import subprocess
import sys

DUMP_SYMS_EXE_PATH = './dump_syms.exe'

def executable_file(filename):
    return filename.endswith('.exe') or\
           filename.endswith('.dll')


def main():
    if len(sys.argv) < 3:
        print('Error: missing build directory or output directory')
        return

    build_dir = sys.argv[1]
    if not os.path.exists(build_dir):
        print('Error: build directory does not exist')
        return

    output_dir = sys.argv[2]
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

    for dir, _, files in os.walk(build_dir):
        for file in files:
            if executable_file(file):
                file_path = os.path.join(dir, file)
                symfile_path = os.path.join(output_dir, file + ".sym")
                print("-> dumping " + file_path)
                with open(symfile_path, "wb") as f:
                    subprocess.call([DUMP_SYMS_EXE_PATH, file_path], stdout=f)

if __name__ == '__main__':
    main()
