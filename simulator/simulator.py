import socket
import random
import time
from collections import deque
random.seed(13)

#-----------------initialization----------------------
width = 6
height = 6
robotLoc = [0, 0]
goalLoc = [width - 1, height - 1]
staticObstacles, dynamicObstacles = [], []
actionQueue = deque(['0','1','2','3','4'])
addObstacles = True
realTimeMode = True
if addObstacles:
    staticObstacles = [[2, 3]]
    dynamicObstacles = [[3, 2],[4,4]]
grid = [['O' for x in range(width)] for y in range(height)]
# controls = 'sudlr'
controls = '01234'
grid[robotLoc[1]][robotLoc[0]] = 'E'
grid[goalLoc[1]][goalLoc[0]] = 'G'
for i in staticObstacles:
    grid[i[1]][i[0]] = 'S'
for i in dynamicObstacles:
    grid[i[1]][i[0]] = 'Y'
#-----------------------open socket----------------------
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 3000)
print ('starting up on %s port %s' % server_address)
sock.bind(server_address)
sock.listen(1)

#---------------------updating functions-------------
def actionFunction(gr, contr, actionQ=False, algo='default'):
    if algo == 'default':
        memo = random.randint(0, len(contr) - 1)
        return contr[memo]
    elif algo == 'client':
        if actionQ:
            return actionQ.popleft()
        else:
            connection, client_address = sock.accept()
            try:
                print('connection from', client_address)

                # Receive the data in small chunks and retransmit it
                # while True:
                data = connection.recv(2048)
                print('received "%s"' % data)
                print("parse value {}".format(data.decode('utf-8')))
                if data:
                    print('sending data back to the client')
                    connection.sendall(data)
            finally:
                # Clean up the connection
                connection.close()
    else:
        return


def easyPlot(gr):
    for i in gr:
        print(' '.join(i))
    print('\n')


def updateLocation(gr, ctrs, act, rloc):
    if act == ctrs[0]:
        pass
    elif act == ctrs[1]:
        if rloc[1] > 0:
            rloc[1] -= 1
    elif act == ctrs[2]:
        if rloc[1] < len(gr) - 1:
            rloc[1] += 1
    elif act == ctrs[3]:
        if rloc[0] > 0:
            rloc[0] -= 1
    elif act == ctrs[4]:
        if rloc[0] < len(gr[0]) - 1:
            rloc[0] += 1

    else:
        # raise ValueError('Motion operators should be in [udlrs]')
        raise ValueError('Motion operators should be in {}'.format(controls))
    return rloc

#------------------main function---------------------------

easyPlot(grid)
for _ in range(80):
    action = actionFunction(grid, controls, actionQueue, 'client')
    print(action)
    grid[robotLoc[1]][robotLoc[0]] = 'O'
    updateLocation(grid, controls, action, robotLoc)
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
        action = actionFunction(grid, controls)
        updateLocation(grid, controls, action, i)
        if grid[i[1]][i[0]] in 'YSG':
            i[1], i[0] = memo[1], memo[0]
        elif i == robotLoc:
            print("Hit an obstacle!")
            # easyPlot(grid)
            break
        grid[i[1]][i[0]] = 'Y'
    easyPlot(grid)
    if realTimeMode:
        time.sleep(0.1)
easyPlot(grid)
