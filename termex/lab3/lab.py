import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.integrate import odeint
import math

def Rot2D(X, Y, Alpha):
    RX = X * np.cos(Alpha) - Y * np.sin(Alpha)
    RY = X * np.sin(Alpha) + Y * np.cos(Alpha)
    return RX, RY

def odesys(y, t, M, m, c, a, b, J_0, g, k):
    dy = np.zeros(4)
    dy[0] = y[2]
    dy[1] = y[3]

    J = J_0 + m * (b**2 + y[0]**2)

    a11 = m*b
    a12 = J
    a21 = 1
    a22 = b

    b1 = ((M*a + m*b) * np.sin(y[1]) + m*y[0]*np.cos(y[1]))*g-c*y[1]-2*m*y[0]*y[2]*y[3]
    b2 = g*np.sin(y[1]) - k/m*y[2]+y[0]*(y[3]**2)

    dy[2] = (b1*a22 - b2*a12)/(a11*a22 - a12*a21)
    dy[3] = (b2*a11 - b1*a21)/(a11*a22 - a12*a21)
    return dy

M = 50
m = 20
a = 1
b = 1.5
# a = 6
# b = 9
J_0 = 100
c = 10000
k = 50
t_0 = 0
phi_0 = np.pi / 20
s_0 = 0
dphi_0 = 0
ds_0 = 0
g = 9.81


Steps = 1001
t_fin = 20
t = np.linspace(0, t_fin, Steps)

y0 = [s_0, phi_0, ds_0, dphi_0]

Y = odeint(odesys, y0, t, (M, m, c, a, b, J_0, g, k))

s = Y[:, 0]
phi = Y[:, 1]
ds = Y[:, 2]
dphi = Y[:, 3]
dds = [odesys(y, t, M, m, c, a, b, J_0, g, k)[2] for y, t in zip(Y, t)]
ddphi = [odesys(y, t, M, m, c, a, b, J_0, g, k)[3] for y, t in zip(Y, t)]

fig_for_graphs = plt.figure(figsize=[13, 7])
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 1)
ax_for_graphs.plot(t, s, color='blue')
ax_for_graphs.set_title("s(t)")
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 2)
ax_for_graphs.plot(t, phi, color='red')
ax_for_graphs.set_title('phi(t)')
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 3)
ax_for_graphs.plot(t, ds, color='green')
ax_for_graphs.set_title("s'(t)")
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 4)
ax_for_graphs.plot(t, dphi, color='black')
ax_for_graphs.set_title('phi\'(t)')
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 5)
ax_for_graphs.plot(t, dds, color='green')
ax_for_graphs.set_title("s''(t)")
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 6)
ax_for_graphs.plot(t, ddphi, color='black')
ax_for_graphs.set_title('phi\'\'(t)')
ax_for_graphs.set(xlim=[0, t_fin])
ax_for_graphs.grid(True)

otstupX = 6
otstupY = 1

BoxX = (b - a) * 6
BoxY = (b - a) * 2
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

X_A = otstupX - a * np.sin(phi)
Y_A = otstupY + a * np.cos(phi)
X_B = otstupX - b * np.sin(phi)
Y_B = otstupY + b * np.cos(phi)

X_Ground = [0, 0, 15]
Y_Ground = [7, 0, 0]

fig = plt.figure(figsize=[15, 7])

ax = fig.add_subplot(1, 1, 1)
ax.axis('equal')
ax.set(xlim=[0, 12], ylim=[-4, 10])

ax.plot(X_Ground, Y_Ground, color='black', linewidth=3)

Drawed_Box = ax.plot(X_A[0]+X_Box[:, 0], Y_A[0]+Y_Box[:, 0], color="#009999")[0]
Drawed_Block = ax.plot(X_B[0]+X_Block[:, 0]+s[0]*np.cos(phi[0]), Y_B[0]+Y_Block[:, 0]+s[0]*np.sin(phi[0]), color="#FF7400")[0]

Nv = 3
R1 = otstupY / 20
R2 = otstupY / 3
thetta = np.linspace(0, Nv*6.28-phi[0], 100)
X_SpiralSpr = -(R1 + thetta*(R2-R1)/thetta[-1])*np.sin(thetta)
Y_SpiralSpr = (R1 + thetta*(R2-R1)/thetta[-1])*np.cos(thetta)
Drawed_SpiralSpring = ax.plot(X_SpiralSpr + otstupX, Y_SpiralSpr+otstupY, color="#00CC00")[0]

Point_A = ax.plot(X_A[0], Y_A[0], marker='o', color='green', markersize='3')[0]
Point_B = ax.plot(X_B[0], Y_B[0], marker='o', color='red', markersize='3')[0]
Line1 = ax.plot([otstupX, X_A[0] + X_Box[4][0]+(X_Box[3][0]-X_Box[4][0])/3],
                [otstupY, Y_A[0]+Y_Box[4][0]+(Y_Box[3][0]-Y_Box[4][0])/3], color="#006363")[0]
Line2 = ax.plot([otstupX, X_A[0] + X_Box[4][0]+2*(X_Box[3][0]-X_Box[4][0])/3],
                  [otstupY, Y_A[0]+Y_Box[4][0]+2*(Y_Box[3][0]-Y_Box[4][0])/3], color="#006363")[0]
Triangle = ax.plot([otstupX - 1, otstupX, otstupX + 1, otstupX-1], [0, otstupY, 0, 0], color="#006363")

#Вектор скорости тела
ArrowX = np.array([-0.1, 0, -0.1])
ArrowY = np.array([0.04, 0, -0.04])
DS = ax.plot([X_B[0]+s[0]*np.cos(phi[0]), X_B[0]+s[0]*np.cos(phi[0]) + ds[0] * np.cos(phi[0])],
             [Y_B[0]+ Block_size/2+s[0]*np.sin(phi[0]), Y_B[0]+ Block_size/2+s[0]*np.sin(phi[0]) + ds[0] *np.sin(phi[0])],
             color="#FF7400")[0]
RArrowX_DS, RArrowY_DS = Rot2D(ArrowX, ArrowY, math.atan2(ds[0]*np.sin(phi[0]), ds[0]*np.cos(phi[0])))
VArrow_DS = ax.plot(RArrowX_DS+X_B[0]+s[0]*np.cos(phi[0])+ds[0]*np.cos(phi[0]),
                    RArrowY_DS+Y_B[0]+Block_size/2+s[0]*np.sin(phi[0])+ds[0]*np.sin(phi[0]),
                 color="#FF7400", label="$ds$")[0]

#Вектор скорости платформы
DPhi = ax.plot([X_A[0], X_A[0]+dphi[0]*np.cos(phi[0])],
             [Y_A[0], Y_A[0]+dphi[0]*np.sin(phi[0])],
             color="#FF0000")[0]
RArrowX_DPhi, RArrowY_DPhi = Rot2D(ArrowX, ArrowY, math.atan2(dphi[0]*np.sin(phi[0]), dphi[0]*np.cos(phi[0])))
VArrow_DPhi = ax.plot(RArrowX_DPhi+X_A[0]+dphi[0]*np.cos(phi[0]),
                    RArrowY_DPhi+Y_A[0]+dphi[0]*np.sin(phi[0]),
                 color="#FF0000", label="$dphi$")[0]

#Вектор ускорения платформы
DDPhi = ax.plot([X_A[0], X_A[0]+ddphi[0]*np.cos(phi[0])],
             [Y_A[0], Y_A[0]+ddphi[0]*np.sin(phi[0])],
             color="#33CCCC", alpha=0.6)[0]
RArrowX_DDPhi, RArrowY_DDPhi = Rot2D(ArrowX, ArrowY, math.atan2(ddphi[0]*np.sin(phi[0]), ddphi[0]*np.cos(phi[0])))
VArrow_DDPhi = ax.plot(RArrowX_DDPhi+X_A[0]+ddphi[0]*np.cos(phi[0]),
                    RArrowY_DDPhi+Y_A[0]+ddphi[0]*np.sin(phi[0]),
                 color="#33CCCC", label="$ddphi$", alpha=0.6)[0]

#Вектор ускорения тела
DDS = ax.plot([X_B[0]+s[0]*np.cos(phi[0]), X_B[0]+s[0]*np.cos(phi[0]) + dds[0] * np.cos(phi[0])],
             [Y_B[0]+ Block_size/2+s[0]*np.sin(phi[0]), Y_B[0]+ Block_size/2+s[0]*np.sin(phi[0]) + dds[0] *np.sin(phi[0])],
             color='#67E667', alpha=0.6)[0]
RArrowX_DDS, RArrowY_DDS = Rot2D(ArrowX, ArrowY, math.atan2(dds[0]*np.sin(phi[0]), dds[0]*np.cos(phi[0])))
VArrow_DDS = ax.plot(RArrowX_DDS+X_B[0]+s[0]*np.cos(phi[0])+dds[0]*np.cos(phi[0]),
                    RArrowY_DDS+Y_B[0]+Block_size/2+s[0]*np.sin(phi[0])+dds[0]*np.sin(phi[0]),
                 color='#67E667', label="$dds$", alpha=0.6)[0]

def anima(i):
    Point_A.set_data(X_A[i], Y_A[i])
    Point_B.set_data(X_B[i], Y_B[i])
    Drawed_Box.set_data(X_A[i]+X_Box[:, i], Y_A[i]+Y_Box[:, i])
    Drawed_Block.set_data(X_B[i] + X_Block[:, i] + s[i] * np.cos(phi[i]), Y_B[i] + Y_Block[:, i] + s[i] * np.sin(phi[i]))

    thetta = np.linspace(0, Nv * 6.28 - phi[i], 100)
    X_SpiralSpr = -(R1 + thetta * (R2 - R1) / thetta[-1]) * np.sin(thetta)
    Y_SpiralSpr = (R1 + thetta * (R2 - R1) / thetta[-1]) * np.cos(thetta)
    Drawed_SpiralSpring.set_data(X_SpiralSpr + otstupX, Y_SpiralSpr+otstupY)
    Line1.set_data([otstupX, X_A[i] + X_Box[4][i] + (X_Box[3][i] - X_Box[4][i]) / 3],
                    [otstupY, Y_A[i] + Y_Box[4][i] + (Y_Box[3][i] - Y_Box[4][i]) / 3])
    Line2.set_data([otstupX, X_A[i] + X_Box[4][i] + 2 * (X_Box[3][i] - X_Box[4][i]) / 3],
                    [otstupY, Y_A[i] + Y_Box[4][i] + 2 * (Y_Box[3][i] - Y_Box[4][i]) / 3])

    # Вектор скорости тела

    DS.set_data([X_B[i] + s[i] * np.cos(phi[i]), X_B[i] + s[i] * np.cos(phi[i]) + ds[i] *np.cos(phi[i])],
                 [Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]),
                  Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]) + ds[i] *np.sin(phi[i])])
    RArrowX_DS, RArrowY_DS = Rot2D(ArrowX, ArrowY, math.atan2(ds[i] * np.sin(phi[i]), ds[i] * np.cos(phi[i])))
    VArrow_DS.set_data(RArrowX_DS + X_B[i] + s[i] * np.cos(phi[i]) + ds[i] * np.cos(phi[i]),
                      RArrowY_DS+Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]) + ds[i] * np.sin(phi[i]))

    # Вектор скорости платформы
    DPhi.set_data([X_A[i], X_A[i] + dphi[i] * np.cos(phi[i])],
                   [Y_A[i], Y_A[i] + dphi[i] * np.sin(phi[i])])
    RArrowX_DPhi, RArrowY_DPhi = Rot2D(ArrowX, ArrowY, math.atan2(dphi[i] * np.sin(phi[i]), dphi[i] * np.cos(phi[i])))
    VArrow_DPhi.set_data(RArrowX_DPhi + X_A[i] + dphi[i] * np.cos(phi[i]),
                          RArrowY_DPhi + Y_A[i] + dphi[i] * np.sin(phi[i]))

    # Вектор ускорения платформы
    DDPhi.set_data([X_A[i], X_A[i] + ddphi[i] * np.cos(phi[i])],
                    [Y_A[i], Y_A[i] + ddphi[i] * np.sin(phi[i])])
    RArrowX_DDPhi, RArrowY_DDPhi = Rot2D(ArrowX, ArrowY,
                                         math.atan2(ddphi[i] * np.sin(phi[i]), ddphi[i] * np.cos(phi[i])))
    VArrow_DDPhi.set_data(RArrowX_DDPhi + X_A[i] + ddphi[i] * np.cos(phi[i]),
                           RArrowY_DDPhi + Y_A[i] + ddphi[i] * np.sin(phi[i]))

    # Вектор ускорения тела
    DDS.set_data([X_B[i] + s[i] * np.cos(phi[i]), X_B[i] + s[i] * np.cos(phi[i]) + dds[i] * np.cos(phi[i])],
                  [Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]),
                   Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]) + dds[i] * np.sin(phi[i])])
    RArrowX_DDS, RArrowY_DDS = Rot2D(ArrowX, ArrowY, math.atan2(dds[i] * np.sin(phi[i]), dds[i] * np.cos(phi[i])))
    VArrow_DDS.set_data(RArrowX_DDS + X_B[i] + s[i] * np.cos(phi[i]) + dds[i] * np.cos(phi[i]),
                         RArrowY_DDS + Y_B[i] + Block_size / 2 + s[i] * np.sin(phi[i]) + dds[i] * np.sin(phi[i]))

    return [Point_A, Point_B, Drawed_SpiralSpring, Drawed_Box, Drawed_Block, Line1, Line2, DS, VArrow_DS, DPhi, RArrowX_DPhi, VArrow_DDPhi, DDPhi, DDS, VArrow_DDS]

anim = FuncAnimation(fig, anima, frames=len(t), interval=10)

plt.legend()
plt.show()