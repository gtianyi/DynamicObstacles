import tkinter as tk

def create_grid(event=None):
    w = c.winfo_width() # Get current width of canvas
    h = c.winfo_height() # Get current height of canvas
    c.delete('grid_line') # Will only remove the grid_line

    # Creates all vertical lines at intevals of 100
    for i in range(0, w, 50):
        c.create_line([(i, 0), (i, h)], tag='grid_line')

    # Creates all horizontal lines at intevals of 100
    for i in range(0, h, 50):
        c.create_line([(0, i), (w, i)], tag='grid_line')

def _create_circle(self, x, y, r, **kwargs):
    return self.create_oval(x-r, y-r, x+r, y+r, **kwargs)
tk.Canvas.create_circle = _create_circle

root = tk.Tk()

c = tk.Canvas(root, height=500 , width=500, bg='white')
c.pack(fill=tk.BOTH, expand=True)

# c.create_circle(25, 25, 25, fill="blue", outline="#DDD", width=4)
for i in range(50):
    c.create_circle(i*5+5, i*5+5, 25, fill="blue", outline="#DDD", width=4)

c.bind('<Configure>', create_grid)

root.mainloop()
