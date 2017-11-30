import socket
import random
import time
from collections import deque
import argparse

PRINTOUT = True
SEEDED = False

class Grid:
    def __init__(self, row=6, col=6, sta = 2, dyn = 2):
        self.win = 0
        self.width = row
        self.height = col
        self.agent = Agent(0, 0)
        self.goal = Object(self.width - 1, self.height - 1)
        self.staticObstacles = []
        self.dynamicObstacles = []
        self.addObstacles(sta,dyn)
        self.tempObstacle = None

    def generateLocation(self):
        return (random.randint(0,self.width-1),random.randint(0,self.height-1))

    def addObstacles(self, sta=2, dyn=2):
        if sta == 2 and dyn == 2:
            self.staticObstacles.append(StaticObstacle(2, 3))
            self.staticObstacles.append(StaticObstacle(5, 1))
            self.dynamicObstacles.append(DynamicObstacle(3, 2))
            self.dynamicObstacles.append(DynamicObstacle(4, 4))
        else:
            cache = set([tuple(self.agent.getLocation()), tuple(self.goal.getLocation())])
            while len(self.staticObstacles) < sta:
                temp = self.generateLocation()
                while temp in cache:
                    temp = self.generateLocation()
                self.staticObstacles.append(StaticObstacle(temp[0],temp[1]))
                cache.add(temp)
                print(temp)
            while len(self.dynamicObstacles) < dyn:
                temp = self.generateLocation()
                while temp in cache:
                    temp = self.generateLocation()
                self.dynamicObstacles.append(DynamicObstacle(temp[0], temp[1]))
                cache.add(temp)

    def updateAll(self):
        self.agent.update()
        self.checkAgent(self.agent)

        for i in self.dynamicObstacles:
            i.update()
            self.checkObstacle(i)

        if self.win == 0:
            if self.tempObstacle:
                if self.tempObstacle.getLocation() == self.agent.getLastLocation():
                    self.win = -1
            self.tempObstacle = None
            if not self.detectObstacles(self.agent):
                self.win = -1

    def checkAgent(self, obj):
        if self.reachGoal(obj):
            self.win = 1
            return
        if not self.isValid(obj):
            self.win = -1
            return
        self.detectObstacles(obj)
        return

    def checkObstacle(self, obj):
        if self.reachGoal(obj):
            obj.revoke()
            return
        if not self.isValid(obj):
            obj.revoke()
            return
        if not self.detectObstacles(obj):
            obj.revoke()
            return
        return

    def detectObstacles(self, obj):
        obstacles = self.dynamicObstacles + self.staticObstacles
        for i in obstacles:
            if obj is i:
                pass
            else:
                if i.getLocation() == obj.getLocation():
                    if obj is self.agent:
                        if i in self.dynamicObstacles:
                            self.tempObstacle = i
                        else:
                            self.win = -1
                        return False
                    else:
                        return False
        return True

    def isValid(self, obj):
        x, y = obj.getLocation()
        if 0 <= x <= self.width - 1 and 0 <= y <= self.height - 1:
            return True
        else:
            return False

    def reachGoal(self, obj):
        if obj.getLocation() == self.goal.getLocation():
            return True
        else:
            return False

    def dump(self, flag='update'):
        result = []
        if flag == 'init':
            result.extend([self.width, self.height])
            result.extend([len(self.staticObstacles), len(self.dynamicObstacles)])
        result.extend(self.agent.getLocation())
        for i in self.dynamicObstacles:
            result.extend(i.getLocation())
        if flag == 'init':
            for i in self.staticObstacles:
                result.extend(i.getLocation())
            result.extend(self.goal.getLocation())
        result = ' '.join([str(x) for x in result])
        return result

    def simplePlot(self):
        cache = {}
        cache[tuple(self.agent.getLocation())] = 'A'
        cache[tuple(self.goal.getLocation())] = 'G'
        for i in self.dynamicObstacles:
            cache[tuple(i.getLocation())] = 'D'
        for i in self.staticObstacles:
            cache[tuple(i.getLocation())] = 'S'
        pass
        for i in range(self.height):
            for j in range(self.width):
                if (j, i) in cache:
                    print(cache[(j, i)], end=" ")
                else:
                    print(".", end=" " )
            print('')
        print('')


class Object:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y
        self.locationHistory = [[self.x, self.y]]

    def getLocation(self):
        return [self.x, self.y]

    def getAction(self):
        return '0'

    def move(self, action):
        """ $ s u d r l 
            $ 0 1 2 3 4"""
        if action == '0':
            pass
        elif action == '1':
            self.y -= 1
        elif action == '2':
            self.y += 1
        elif action == '3':
            self.x += 1
        elif action == '4':
            self.x -= 1
        else:
            self.x = -1
            self.y = -1

    def update(self):
        self.locationHistory.append(self.getLocation())
        self.move(self.getAction())


class Agent(Object):
    def __init__(self, x, y):
        Object.__init__(self, x, y)
        # self.actionQueue = deque(['2', '2', '0', '3', '3', '3', '3'])
        self.actionQueue = deque()
        self.locationHistory = []
        pass

    def getAction(self):
        if self.actionQueue:
            temp = self.actionQueue.popleft()
            if PRINTOUT:
                print(temp)
            return temp
        else:
            return -1

    def getLastLocation(self):
        return self.locationHistory[-1]

    def fetchActions(self, data):
        self.actionQueue += data.split(' ')


class DynamicObstacle(Object):
    def __init__(self, x, y):
        Object.__init__(self, x, y)

    def getAction(self):
        return str(random.randint(0, 4))

    def revoke(self):
        self.x = self.locationHistory[-1][0]
        self.y = self.locationHistory[-1][1]


class StaticObstacle(Object):
    def __init__(self, x, y):
        Object.__init__(self, x, y)

def createSocket( port ):
    sock, connection = 0, 0
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2048)
    server_address = ('localhost', port)
    print ('starting up on %s port %s' % server_address)
    sock.bind(server_address)
    sock.listen(1)
    connection, _ = sock.accept()
    return connection


if __name__ == "__main__":
    timeframe = 0.5
    PRINTOUT = False
    SEEDED = False

    parser = argparse.ArgumentParser(description='Start a simulator training server.')
    parser.add_argument('-s','--seed', action='store_true',default=False, help='Import seed for random numbers')
    parser.add_argument('-p','--plot', action='store_true',default=False,help='Turn on simple plot')
    args = parser.parse_args()
    PRINTOUT = vars(args)['plot']
    SEEDED = vars(args)['seed']
    # set Grid parameters in the next line. Grid(10,10,5,3) means 10 by 10 map with 5 static and 3 dynamic obstacles
    game = Grid()
    if PRINTOUT:
        game.simplePlot()
    connection = createSocket(3000)
    data = connection.recv(2048).decode("utf-8")
    if data == "start":
        connection.sendall(game.dump('init').encode("utf-8"))

    while True:
        if SEEDED:
            random.seed(13)
        game = Grid()

        while True:
            if not game.agent.actionQueue:
                data = connection.recv(2048).decode("utf-8")
                game.agent.fetchActions(data)
            game.updateAll()
            if PRINTOUT:
                game.simplePlot()
            if game.win == 1:
                if PRINTOUT:
                    print("win")
                connection.sendall("win".encode("utf-8"))
                break
            elif game.win == -1:
                if PRINTOUT:
                    print("lose")
                connection.sendall("lose".encode("utf-8"))
                break
            elif game.win == 0:
                connection.sendall(game.dump().encode("utf-8"))
