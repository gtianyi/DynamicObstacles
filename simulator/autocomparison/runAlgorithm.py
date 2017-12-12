size = 10
program = "./alg"
number = 60000
file = "./result/q_10_6.log"

numbers = [1000,5000,10000]#,20000,30000,60000]
vers = ['0.1','0.5','1']#,'2','3','6']

import sys
import os
import re
import shutil
import time

def run(command):
    ret_val = os.system(command)
    if ret_val != 0:
        print('FAILED: command=\'' + command + '\'')
        sys.exit(1)

for size in range(6,11):
    for i in range(len(numbers)):
        number = numbers[i]
        file = "./result/t_"+str(size)+"_"+vers[i]+".log"
        comm =  program + " -a uct -d 100" + " -n "+str(number) + " >> "+file
        for _ in range(10):
            try:
                print('running ' + comm)
                run(command=comm)
            except:
                pass
                print("Next one")

            time.sleep(1)