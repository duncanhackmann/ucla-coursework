class Point:
    def __init__(self, x, y):
        self.x_ = x
        self.y_ = y

    def display(self):
        print(self.x_, ",", self.y_)

def modify(p):
    p.x_ = 50          # line A 
    p = Point(20, 30)  # line B


# equivalent of our C++ main() function follows
pt = Point(10, 10)
modify(pt)
pt.display()