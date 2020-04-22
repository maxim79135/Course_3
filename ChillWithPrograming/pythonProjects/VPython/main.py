from vpython import *

scene.range = 100

mybox = box(pos=vector(0,0,0), length=20, height=20, width=20)
mybox.rate(50)


def showSphere(evt):
    loc = evt.pos
    sphere(pos=loc, radius=10,color=color.cyan)

scene.bind('click', showSphere)
