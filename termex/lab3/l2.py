import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import math

def Rot2D(X, Y, Alpha):
    RX = X * np.cos(Alpha) - Y * np.sin(Alpha)
    RY = X * np.sin(Alpha) + Y * np.cos(Alpha)
    return RX, RY

Steps = 1001
t_fin = 20
t = np.linspace(0, t_fin, Steps)
BoxX = 6
BoxY = 3
SubW = 1


phi = np.sin(1.5*t) + 0.25 * np.cos(2.5*t)
s = -phi

otstupX = 8
otstupY = 4

Block_size = BoxX / 8

def to_Ox(X, Y):
    global phi
    return X * np.cos(phi) - Y * np.sin(phi)

def to_Oy(X, Y):
    global phi
    return X * np.sin(phi) + Y * np.cos(phi)

X_Box = np.array([
     to_Ox(-BoxX/2, -BoxY/2),
     to_Ox(-BoxX/2, BoxY/2),
     to_Ox(BoxX/2, BoxY/2),
     to_Ox(BoxX/2, -BoxY/2),
     to_Ox(-BoxX/2, -BoxY/2),
])

Y_Box = np.array([
    to_Oy(-BoxX/2, -BoxY/2),
    to_Oy(-BoxX/2, BoxY/2),
    to_Oy(BoxX/2, BoxY/2),
    to_Oy(BoxX/2, -BoxY/2),
    to_Oy(-BoxX/2, -BoxY/2),
])

X_Block = np.array([
     to_Ox(-Block_size/2, 0),
     to_Ox(-Block_size/2, Block_size),
     to_Ox(Block_size/2, Block_size),
     to_Ox(Block_size/2, 0),
     to_Ox(-Block_size/2, 0),
])

Y_Block = np.array([
    to_Oy(-Block_size/2, 0),
    to_Oy(-Block_size/2, Block_size),
    to_Oy(Block_size/2, Block_size),
    to_Oy(Block_size/2, 0),
    to_Oy(-Block_size/2, 0),
])

X_A = otstupX - BoxY/2 * np.sin(phi)
Y_A = otstupY + BoxY/2 * np.cos(phi)
X_B = otstupX - BoxY * np.sin(phi)
Y_B = otstupY + BoxY * np.cos(phi)

X_Ground = [0, 0, 15]
Y_Ground = [7, 0, 0]

fig = plt.figure(figsize=[15, 7])

ax = fig.add_subplot(1, 1, 1)
ax.axis('equal')
ax.set(xlim=[0, 12], ylim=[-4, 10])

ax.plot(X_Ground, Y_Ground, color='black', linewidth=3)

Drawed_Box = ax.plot(X_A[0]+X_Box[:, 0], Y_A[0]+Y_Box[:, 0], color="#009999")[0]
Drawed_Block = ax.plot(X_B[0]+X_Block[:, 0]+s[0], Y_B[0]+Y_Block[:, 0]+s[0], color="#FF7400")[0]

#Спираль
Nv = 3
R1 = 0.2
R2 = 0.8
thetta = np.linspace(0, Nv*6.28-phi[0], 100)
X_SpiralSpr = -(R1 + thetta*(R2-R1)/thetta[-1])*np.sin(thetta)
Y_SpiralSpr = (R1 + thetta*(R2-R1)/thetta[-1])*np.cos(thetta)
Drawed_SpiralSpring = ax.plot(X_SpiralSpr+otstupX, Y_SpiralSpr+otstupY/2, color="#00CC00")[0]

Point_A = ax.plot(X_A[0], Y_A[0], marker='o', color='green', markersize='3')[0]
Point_B = ax.plot(X_B[0], Y_B[0], marker='o', color='red', markersize='3')[0]
Line1 = ax.plot([otstupX, X_A[0] + X_Box[4][0]+(X_Box[3][0]-X_Box[4][0])/3],
                [otstupY/2, Y_A[0]+Y_Box[4][0]+(Y_Box[3][0]-Y_Box[4][0])/3], color="#006363")[0]
Line2 = ax.plot([otstupX, X_A[0] + X_Box[4][0]+2*(X_Box[3][0]-X_Box[4][0])/3],
                  [otstupY/2, Y_A[0]+Y_Box[4][0]+2*(Y_Box[3][0]-Y_Box[4][0])/3], color="#006363")[0]
Triangle = ax.plot([otstupX - 1, otstupX, otstupX + 1, otstupX-1], [0, otstupY/2, 0, 0], color="#006363")

def anima(i):
    Point_A.set_data(X_A[i], Y_A[i])
    Point_B.set_data(X_B[i], Y_B[i])
    Drawed_Box.set_data(X_A[i]+X_Box[:, i], Y_A[i]+Y_Box[:, i])
    Drawed_Block.set_data(X_B[i] + X_Block[:, i] + s[i] * np.cos(phi[i]), Y_B[i] + Y_Block[:, i] + s[i] * np.sin(phi[i]))

    thetta = np.linspace(0, Nv * 6.28 - phi[i], 100)
    X_SpiralSpr = -(R1 + thetta * (R2 - R1) / thetta[-1]) * np.sin(thetta)
    Y_SpiralSpr = (R1 + thetta * (R2 - R1) / thetta[-1]) * np.cos(thetta)
    Drawed_SpiralSpring.set_data(X_SpiralSpr + otstupX, Y_SpiralSpr+otstupY/2)
    Line1.set_data([otstupX, X_A[i] + X_Box[4][i] + (X_Box[3][i] - X_Box[4][i]) / 3],
                    [otstupY/2, Y_A[i] + Y_Box[4][i] + (Y_Box[3][i] - Y_Box[4][i]) / 3])
    Line2.set_data([otstupX, X_A[i] + X_Box[4][i] + 2 * (X_Box[3][i] - X_Box[4][i]) / 3],
                    [otstupY/2, Y_A[i] + Y_Box[4][i] + 2 * (Y_Box[3][i] - Y_Box[4][i]) / 3])

    return [Point_A, Point_B, Drawed_SpiralSpring, Drawed_Box, Drawed_Block, Line1, Line2]

anim = FuncAnimation(fig, anima, frames=len(t), interval=10)

plt.legend()
plt.show()