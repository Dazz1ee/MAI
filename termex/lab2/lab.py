import math

import numpy as np
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import sympy as sp

t = sp.Symbol('t')

def Rot2D(X, Y, Alpha):
    RX = X * np.cos(Alpha) - Y * np.sin(Alpha)
    RY = X * np.sin(Alpha) + Y * np.cos(Alpha)
    return RX, RY

BoxX = 8
BoxY = 4
SubW = 1
SpaceY = 4
SpaceX = 8
k = -1

phi = sp.sin(1.7*t) + 0.25 * sp.cos(2.5*t)
u = sp.diff(phi, t)
s = k*u

tn = np.linspace(0, 20, 1000)

def to_Ox(X, Y):
    global phi, BoxX, SpaceX
    return X * sp.cos(phi) - Y * sp.sin(phi) + BoxX / 2 + SpaceX

def to_Oy(X, Y):
    global phi, SpaceY
    return X * sp.sin(phi) + Y * sp.cos(phi) + SpaceY

X_Box = [
     to_Ox(-BoxX/2, 1),
     to_Ox(-BoxX/2, 1+BoxY),
     to_Ox(BoxX/2, 1+BoxY),
     to_Ox(BoxX/2, 1),
     to_Ox(-BoxX/2, 1)
]

Y_Box = [
    to_Oy(-BoxX/2, 1),
    to_Oy(-BoxX/2, 1+BoxY),
    to_Oy(BoxX/2, 1+BoxY),
    to_Oy(BoxX/2, 1),
    to_Oy(-BoxX/2, 1)
]

X_Sub = [
    to_Ox(-s-SubW, 1+BoxY),
    to_Ox(-s-SubW, 1+BoxY+SubW),
    to_Ox(-s, 1+BoxY+SubW),
    to_Ox(-s, 1 + BoxY),
    to_Ox(-s-SubW, 1+BoxY),
]

Y_Sub = [
    to_Oy(-SubW, 1+BoxY) - s * sp.sin(phi),
    to_Oy(-SubW, 1+BoxY+SubW) - s * sp.sin(phi),
    to_Oy(0, 1+BoxY+SubW) - s * sp.sin(phi),
    to_Oy(0, 1+BoxY) - s * sp.sin(phi),
    to_Oy(-SubW, 1+BoxY)- s * sp.sin(phi)
]

X_O = SpaceX + BoxX / 2
Y_O = SpaceY / 2

#Точка A
x_a = to_Ox(0, 1+BoxY / 2)
y_a = to_Oy(0, 1+BoxY / 2)
vx_a = sp.diff(x_a, t)
vy_a = sp.diff(y_a, t)

#Точка D
x_d = to_Ox(-s-SubW/2, SubW+BoxY+SubW/2)
y_d = to_Oy(-s-SubW/2, SubW+BoxY+SubW/2)
vx_d = sp.diff(x_d, t)
vy_d = sp.diff(y_d, t)

Phi = np.zeros_like(tn)
VY_A = np.zeros_like(tn)
VX_A = np.zeros_like(tn)
VY_D = np.zeros_like(tn)
VX_D = np.zeros_like(tn)
# U = np.zeros_like(tn)
X_A = np.zeros_like(tn)
Y_A = np.zeros_like(tn)
X_D = np.zeros_like(tn)
Y_D = np.zeros_like(tn)
Box_X = []
Box_Y = []
Sub_X = []
Sub_Y = []
for i in range(5):
    Box_X.append(np.zeros_like(tn))
    Box_Y.append(np.zeros_like(tn))
    Sub_Y.append(np.zeros_like(tn))
    Sub_X.append(np.zeros_like(tn))

for i in range(len(tn)):
    Phi[i] = sp.Subs(phi, t, tn[i])
    # U[i] = sp.Subs(u, t, tn[i])
    X_A[i] = sp.Subs(x_a, t, tn[i])
    Y_A[i] = sp.Subs(y_a, t, tn[i])
    X_D[i] = sp.Subs(x_d, t, tn[i])
    Y_D[i] = sp.Subs(y_d, t, tn[i])
    VY_A[i] = sp.Subs(vy_a, t, tn[i])
    VX_A[i] = sp.Subs(vx_a, t, tn[i])
    VY_D[i] = sp.Subs(vy_d, t, tn[i])
    VX_D[i] = sp.Subs(vx_d, t, tn[i])
    Box_X[0][i] = sp.Subs(X_Box[0], t, tn[i])
    Box_Y[0][i] = sp.Subs(Y_Box[0], t, tn[i])
    Box_X[1][i] = sp.Subs(X_Box[1], t, tn[i])
    Box_Y[1][i] = sp.Subs(Y_Box[1], t, tn[i])
    Box_X[2][i] = sp.Subs(X_Box[2], t, tn[i])
    Box_Y[2][i] = sp.Subs(Y_Box[2], t, tn[i])
    Box_X[3][i] = sp.Subs(X_Box[3], t, tn[i])
    Box_Y[3][i] = sp.Subs(Y_Box[3], t, tn[i])
    Box_X[4][i] = sp.Subs(X_Box[4], t, tn[i])
    Box_Y[4][i] = sp.Subs(Y_Box[4], t, tn[i])
    Sub_X[0][i] = sp.Subs(X_Sub[0], t, tn[i])
    Sub_Y[0][i] = sp.Subs(Y_Sub[0], t, tn[i])
    Sub_X[1][i] = sp.Subs(X_Sub[1], t, tn[i])
    Sub_Y[1][i] = sp.Subs(Y_Sub[1], t, tn[i])
    Sub_X[2][i] = sp.Subs(X_Sub[2], t, tn[i])
    Sub_Y[2][i] = sp.Subs(Y_Sub[2], t, tn[i])
    Sub_X[3][i] = sp.Subs(X_Sub[3], t, tn[i])
    Sub_Y[3][i] = sp.Subs(Y_Sub[3], t, tn[i])
    Sub_X[4][i] = sp.Subs(X_Sub[4], t, tn[i])
    Sub_Y[4][i] = sp.Subs(Y_Sub[4], t, tn[i])
X_Ground = [0, 0, 30]
Y_Ground = [14, 0, 0]


fig = plt.figure()

ax = fig.add_subplot(1, 1, 1)
ax.axis('equal')
ax.set(xlim=[0, 24], ylim=[-4, 20])

ax.plot(X_Ground, Y_Ground, color='black', linewidth=3)

#Тело(Rectangle) и Точка (Sub)
Rectangle = ax.plot([Box_X[0][0], Box_X[1][0], Box_X[2][0], Box_X[3][0], Box_X[4][0]],
                  [Box_Y[0][0], Box_Y[1][0], Box_Y[2][0], Box_Y[3][0], Box_Y[4][0]], color="blue")[0]


Sub = ax.plot([Sub_X[0][0], Sub_X[1][0], Sub_X[2][0], Sub_X[3][0], Sub_X[4][0]],
                  [Sub_Y[0][0], Sub_Y[1][0], Sub_Y[2][0], Sub_Y[3][0], Sub_Y[4][0]], color="orange")[0]

Point1 = ax.plot(X_O, Y_O, marker="o")
Triangle1 = ax.plot([X_O - 2, X_O, X_O + 2, X_O-2], [0, Y_O, 0, 0], color="purple")
Line1 = ax.plot([X_O, Box_X[4][0] + 2 * (Box_X[3][0] - Box_X[4][0]) / 5], [Y_O, 2 * (Box_Y[3][0] - Box_Y[4][0]) / 5 + Box_Y[4][0]], color="purple")[0]
Line2 = ax.plot([X_O, Box_X[4][0] + 3 * (Box_X[3][0] - Box_X[4][0]) / 5], [Y_O, 3 * (Box_Y[3][0] - Box_Y[4][0]) / 5 + Box_Y[4][0]], color="purple")[0]
Point2, = ax.plot(X_A[0], Y_A[0], marker="o")

#Спираль
Nv = 3
R1 = 0.2
R2 = 1
thetta = np.linspace(0, Nv*6.28-Phi[0], 100)
X_SpiralSpr = -(R1 + thetta*(R2-R1)/thetta[-1])*np.sin(thetta)
Y_SpiralSpr = (R1 + thetta*(R2-R1)/thetta[-1])*np.cos(thetta)
Drawed_SpiralSpring = ax.plot(X_SpiralSpr+X_O, Y_SpiralSpr+Y_O)[0]


#Вектор скорости точки A
ArrowX = np.array([-0.2, 0, -0.2])
ArrowY = np.array([0.1, 0, -0.1])
VLine, = ax.plot([X_A[0],  X_A[0] + VX_A[0]],
                [Y_A[0],  Y_A[0] + VY_A[0]], label='$скорость \\ платформы$', color="red")
RArrowX, RArrowY = Rot2D(ArrowX, ArrowY, math.atan2(VY_A[0], VX_A[0]))
VArrow, = ax.plot(RArrowX + X_A[0] + VX_A[0],
                    RArrowY + Y_A[0] + VY_A[0], color="red")

#Вектор скорости точки D
VLine_D, = ax.plot([X_D[0],  X_D[0] + VX_D[0]],
                [Y_D[0],  Y_D[0] + VY_D[0]] , label='$скорость \\ тела$', color="brown")
RArrowX_D, RArrowY_D = Rot2D(ArrowX, ArrowY, math.atan2(VY_D[0], VX_D[0]))
VArrow_D, = ax.plot(RArrowX_D + X_D[0] + VX_D[0],
                    RArrowY_D + Y_D[0] + VY_D[0], color="brown")
def anima(i):
    Rectangle.set_data([Box_X[0][i], Box_X[1][i], Box_X[2][i], Box_X[3][i], Box_X[4][i]],
                  [Box_Y[0][i], Box_Y[1][i], Box_Y[2][i], Box_Y[3][i], Box_Y[4][i]])
    Sub.set_data([Sub_X[0][i], Sub_X[1][i], Sub_X[2][i], Sub_X[3][i], Sub_X[4][i]],
                       [Sub_Y[0][i], Sub_Y[1][i], Sub_Y[2][i], Sub_Y[3][i], Sub_Y[4][i]])
    Line1.set_data([X_O, Box_X[4][i] + 2 * (Box_X[3][i] - Box_X[4][i]) / 5],
                    [Y_O, 2 * (Box_Y[3][i] - Box_Y[4][i]) / 5 + Box_Y[4][i]])
    Line2.set_data([X_O, Box_X[4][i] + 3 * (Box_X[3][i] - Box_X[4][i]) / 5],
                    [Y_O, 3 * (Box_Y[3][i] - Box_Y[4][i]) / 5 + Box_Y[4][i]])
    thetta = np.linspace(0, Nv * 6.28 - Phi[i], 100)
    X_SpiralSpr = -(R1 + thetta * (R2 - R1) / thetta[-1]) * np.sin(thetta)
    Y_SpiralSpr = (R1 + thetta * (R2 - R1) / thetta[-1]) * np.cos(thetta)
    Drawed_SpiralSpring.set_data(X_SpiralSpr + X_O, Y_SpiralSpr + Y_O)

    # Вектор скорости точки A
    VLine.set_data([X_A[i],  X_A[i] + VX_A[i]],
                [Y_A[i],  Y_A[i] + VY_A[i]])
    RArrowX, RArrowY = Rot2D(ArrowX, ArrowY, math.atan2(VY_A[i], VX_A[i]))
    VArrow.set_data(RArrowX + X_A[i] + VX_A[i],
                    RArrowY + Y_A[i] + VY_A[i])
    Point2.set_data(X_A[i], Y_A[i])

    # Вектор скорости точки D
    VLine_D.set_data([X_D[i],  X_D[i] + VX_D[i]],
                [Y_D[i],  Y_D[i] + VY_D[i]])
    RArrowX_D, RArrowY_D = Rot2D(ArrowX, ArrowY, math.atan2(VY_D[i], VX_D[i]))
    VArrow_D.set_data(RArrowX_D + X_D[i] + VX_D[i],
                    RArrowY_D + Y_D[i] + VY_D[i])

    return Line1, Line2, Rectangle, Point2, Drawed_SpiralSpring, Sub, VLine, VArrow, VLine_D, VArrow_D

anim = FuncAnimation(fig, anima, frames=len(tn), interval=1)

plt.legend()
plt.show()
