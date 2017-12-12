size = 10
program = "simulator_tian.py"

import sys
import os
import re
import shutil

def run(command):
    ret_val = os.system(command)
    if ret_val != 0:
        print('FAILED: command=\'' + command + '\'')
        sys.exit(1)

for size in range(6,11):
    comm = 'python3 ' + program + " -row "+str(size) + " -col "+str(size)
    for _ in range(3):
        for _ in range(10):
            try:
                print('running ' + comm)
                run(command=comm)
            except:
                pass
                print("Next one")