import random
import time
random.seed(13)

#-----------------initialization----------------------
width = 6
height = 6
robotLoc = [0, 0]
goalLoc = [width - 1, height - 1]
staticObstacles, dynamicObstacles = [], []
addObstacles = True
realTimeMode = False
if addObstacles:
    staticObstacles = [[2, 1]]
    dynamicObstacles = [[1, 2]]
grid = [['O' for x in range(width)] for y in range(height)]
controls = 'sudlr'
# controls = '01234'
grid[robotLoc[1]][robotLoc[0]] = 'E'
grid[goalLoc[1]][goalLoc[0]] = 'G'
for i in staticObstacles:
    grid[i[1]][i[0]] = 'S'
for i in dynamicObstacles:
    grid[i[1]][i[0]] = 'Y'


#---------------------updating functions-------------
def actionFunction(gr, con, algo='default'):
    if algo == 'default':
        memo = random.randint(0, len(con) - 1)
        print(con[memo])
        return con[memo]
    else:
        return


def easyPlot(gr):
    for i in gr:
        print(' '.join(i))
    print('\n')


def updateLocation(gr, act, rloc):
    if act == 'u':
        if rloc[1] > 0:
            rloc[1] -= 1
    elif act == 'd':
        if rloc[1] < len(gr) - 1:
            rloc[1] += 1
    elif act == 'l':
        if rloc[0] > 0:
            rloc[0] -= 1
    elif act == 'r':
        if rloc[0] < len(gr[0]) - 1:
            rloc[0] += 1
    elif act == 's':
        pass
    else:
        # raise ValueError('Motion operators should be in [udlrs]')
        raise ValueError('Motion operators should be in {}'.format(controls))
    return rloc

#------------------main function---------------------------

easyPlot(grid)
for _ in range(80):
    action = actionFunction(grid, controls)
    grid[robotLoc[1]][robotLoc[0]] = 'O'
    updateLocation(grid, action, robotLoc)
    grid[robotLoc[1]][robotLoc[0]] = 'E'
    if robotLoc == goalLoc:
        print("Successfully reach the goal!")
        # easyPlot(grid)
        break
    elif robotLoc in staticObstacles or robotLoc in dynamicObstacles:
        print("Hit an obstacle!")
        # easyPlot(grid)
        break
    for i in dynamicObstacles:
        memo = i[:]
        grid[i[1]][i[0]] = 'O'

        updateLocation(grid, action, i)
        if grid[i[1]][i[0]] in 'YSG':
            i[1], i[0] = memo[1], memo[0]
        elif i == robotLoc:
            print("Hit an obstacle!")
            # easyPlot(grid)
            break
        grid[i[1]][i[0]] = 'Y'
    easyPlot(grid)
    if realTimeMode:
        time.sleep(1)
easyPlot(grid)
