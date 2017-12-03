import tkinter as tk
from trainer import *
import time
import random

# random.seed(51)
game = Grid(10, 10, 5, 5)
PRINTOUT = True


def create_grid(event=None):
    w = c.winfo_width()  # Get current width of canvas
    h = c.winfo_height()  # Get current height of canvas
    c.delete('grid_line')  # Will only remove the grid_line

    # Creates all vertical lines at intevals of 100
    for i in range(0, w, w // 10):
        c.create_line([(i, 0), (i, h)], tag='grid_line')

    # Creates all horizontal lines at intevals of 100
    for i in range(0, h, h // 10):
        c.create_line([(0, i), (w, i)], tag='grid_line')


def _create_circle(self, x, y, r, **kwargs):
    return self.create_oval(x - r, y - r, x + r, y + r, **kwargs)


def _create_circle2(self, tup, **kwargs):
    r = 30
    x = tup[0] * 60 + 30
    y = tup[1] * 60 + 30
    return self.create_oval(x - r, y - r, x + r, y + r, **kwargs)


def convertLocation(tup):
    r = 30
    x = tup[0] * 60 + 30
    y = tup[1] * 60 + 30
    return [x - r, y - r, x + r, y + r]


# def _create_circle3(self, tup, **kwargs):
#     w = self.winfo_width()
#     h = self.winfo_height()
#     print(w)
#     rw = w//20
#     rh = h//20
#     x = tup[0] * (w//10) + rw
#     y = tup[1] * (h//10) + rh
#     return self.create_oval(x - rw, y - rh, x + rw, y + rh, **kwargs)

def communicationLoop():
    if not game.agent.actionQueue:
        data = connection.recv(2048).decode("utf-8")
        game.agent.fetchActions(data)
    if game.win == 1:
        if PRINTOUT:
            print("win")
        c.create_text(300, 300, text="Win!", font=("Bodoni", 40), fill="red")
        connection.sendall("win".encode("utf-8"))
        connection.close()
    elif game.win == -1:
        if PRINTOUT:
            print("lose")
        c.create_text(300, 300, text="Lose!", font=("Bodoni", 40), fill="red")
        connection.sendall("lose".encode("utf-8"))
        connection.close()
    elif game.win == 0:
        connection.sendall(game.dump().encode("utf-8"))
    game.updateAll()
    if PRINTOUT:
        game.simplePlot()

    c.coords(agen, convertLocation(game.agent.getLocation()))
    for i in range(len(dyn)):
        c.coords(dyn[i], convertLocation(game.dynamicObstacles[i].getLocation()))

    # print(c.coords(a))
    # t = c.coords(a)
    # t = list(map((lambda x:x+30),t))
    # c.coords(a,t)
    root.after(500, communicationLoop)


tk.Canvas.create_circle = _create_circle2

root = tk.Tk()
root.resizable(False, False)
c = tk.Canvas(root, height=600, width=600, bg='white')
c.pack(fill=tk.BOTH, expand=True)

# c.create_circle(25, 25, 25, fill="blue", outline="#DDD", width=4)
# c.create_circle(game.goal.getLocation()[0] * 50 + 25, game.goal.getLocation()[1] * 50 + 25, 25, fill="green")
agen = c.create_circle(game.agent.getLocation(), fill="red")
dyn = []
c.create_circle(game.goal.getLocation(), fill="green")
for i in game.staticObstacles:
    c.create_circle(i.getLocation(), fill="blue")
for i in game.dynamicObstacles:
    dyn.append(c.create_circle(i.getLocation(), fill="orange"))

c.bind('<Configure>', create_grid)

connection = createSocket(4000)
data = connection.recv(2048).decode("utf-8")
if data == "start":
    connection.sendall(game.dump('init').encode("utf-8"))

communicationLoop()
root.mainloop()
