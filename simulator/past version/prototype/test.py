import argparse
constant_value = 0
parser = argparse.ArgumentParser(description='Start a simulator training server.')
parser.add_argument('-s','--seed', action='store',dest='seed',default = '', help='Import seed for random numbers')
parser.add_argument('-p','--plot', action='store_true',dest='plot',default=False,help='Turn on simple plot')
parser.add_argument('-c','--constant', action = 'store',dest='constant', help = 'input constant')
parser.add_argument('-row', action='store',dest='row',default = '6', help='Import row size for grid')
parser.add_argument('-col', action='store',dest='col',default = '6', help='Import row size for grid')
parser.add_argument('-sta', action='store',dest='sta',default = '2', help='Import number of static obstacles for grid')
parser.add_argument('-dyn', action='store',dest='dyn',default = '2', help='Import number of dynamic obstacles for grid')

args = parser.parse_args()
# parser.add_argument()
PRINTOUT = vars(args)['plot']
SEEDED = vars(args)['seed']
# GRIDINPUT = vars(args)['gridInput']
print(vars(args))
print(PRINTOUT)
if SEEDED:
    print(SEEDED)
# print(GRIDINPUT)
