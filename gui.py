import matplotlib.pyplot as plt
from  matplotlib.widgets import Slider, Button
import numpy as np
import sys


#Cache Size
L1_SIZE=1*1024
L2_SIZE=4*1024
L3_SIZE=32*1024
#open file,read reuse distance
x=[]
y=[]
i=0
for line in open(sys.argv[1],'r'):
	i=i+1
	if(i>=10 and i<10+L3_SIZE):
		tmp = line.strip().split()
		x.append(int(tmp[0]))
		y.append(int(tmp[1]))
#initial plot 
fig = plt.figure()
ax = fig.add_subplot(111)
ax.autoscale(True)

l, = ax.plot(x,y)
ax.axis([0, L3_SIZE, 0, 2 * np.mean(y)])
plt.xlabel("Reuse Distance")
plt.ylabel("Memory Access")
plt.title("Histogram")
axcolor = 'lightgoldenrodyellow'

#Button Object
ax_levelt  = plt.axes([0.19, 0.04, 0.03, 0.03], axisbg=axcolor)
ax_level1  = plt.axes([0.1 , 0.04, 0.03, 0.03], axisbg=axcolor)
ax_level2  = plt.axes([0.13, 0.04, 0.03, 0.03], axisbg=axcolor)
ax_level3  = plt.axes([0.16, 0.04, 0.03, 0.03], axisbg=axcolor)
b_levelt   = Button(ax_levelt,  'total ', color=axcolor, hovercolor='0.975')
b_level1   = Button(ax_level1,  'Level1', color=axcolor, hovercolor='0.975')
b_level2   = Button(ax_level2,  'Level2', color=axcolor, hovercolor='0.975')
b_level3   = Button(ax_level3,  'Level3', color=axcolor, hovercolor='0.975')
now_level  = 0
def click_levelt(event):
	global now_level
	now_level = 0
	l.set_xdata(x)
	l.set_ydata(y)	
	ax.set_xlim([0, L3_SIZE])
	ax.set_ylim([0, 2*int(np.mean(y[0:L3_SIZE]))])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level1(event):
	global now_level
	now_level = 1
	l.set_xdata(x[0:L1_SIZE])
	l.set_ydata(y[0:L1_SIZE])	
	ax.set_xlim([0, L1_SIZE])
	ax.set_ylim([0, 2*int(np.mean(y[0:L1_SIZE]))])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level2(event):
	global now_level
	now_level = 2 
	l.set_xdata(x[L1_SIZE:L2_SIZE])
	l.set_ydata(y[L1_SIZE:L2_SIZE])	
	ax.set_xlim([L1_SIZE, L2_SIZE])
	ax.set_ylim([0, 2*int(np.mean(y[0:L2_SIZE]))])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level3(event):
	global now_level
	now_level = 3
	l.set_xdata(x[L2_SIZE:L3_SIZE])
	l.set_ydata(y[L2_SIZE:L3_SIZE])	
	ax.set_xlim([L2_SIZE, L3_SIZE])
	ax.set_ylim([0, 2*int(np.mean(y[0:L3_SIZE]))])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
b_levelt.on_clicked(click_levelt)
b_level1.on_clicked(click_level1)
b_level2.on_clicked(click_level2)
b_level3.on_clicked(click_level3)


#Slider
ax_access  = plt.axes([0.25, 0.04, 0.03 , 0.03], axisbg=axcolor)
s_access  = Slider(ax_access, 'access', 0, 10, valinit=0,valfmt='%d')
def update(val):
	if(now_level == 0):
		ax.set_ylim([0, 2*np.mean(y[0:L3_SIZE])*(2**s_access.val)])		
	elif(now_level == 1):
		ax.set_ylim([0, 2*np.mean(y[0:L1_SIZE])*(2**s_access.val)])
	elif(now_level == 2):
		ax.set_ylim([0, 2*np.mean(y[L1_SIZE:L2_SIZE])*(2**s_access.val)])
	elif(now_level == 3):
		ax.set_ylim([0, 2*np.mean(y[L2_SIZE:L3_SIZE])*(2**s_access.val)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
s_access.on_changed(update)


plt.show()
