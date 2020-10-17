import numpy as np
import sys

import matplotlib
from matplotlib import pyplot as plt
from matplotlib import animation
matplotlib.use("Qt5Agg")

file = sys.argv[1]
fig = plt.figure()
ax = fig.add_subplot(111, autoscale_on=False, xlim=(-3, 3), ylim=(-3, 3))
ax.set_aspect('equal')
ax.grid()

line, = ax.plot([],[], 'o-',lw=2)

[t,cartx1,carty1,cartx2,carty2] = np.loadtxt(file, delimiter='\t', usecols=[0,5,6,7,8],unpack=True)

#def init():
#		line.set_data([],[])
#		return line,

#def animate(i):
#		return 0

def init():
		line.set_data([], [])
		return line, 

def animate(i):
		thiscartx = [0, cartx1[i], cartx2[i]]
		thiscarty = [0, carty1[i], carty2[i]]
		line.set_data(thiscartx, thiscarty)
		return line,

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, frames=len(t), init_func=init, interval=1e-5, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
plt.show()
