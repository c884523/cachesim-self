import matplotlib.pyplot as plt
from  matplotlib.widgets import Slider, Button
import numpy as np
import sys


#Cache Size
L1_SIZE=int(sys.argv[1])
L2_SIZE=int(sys.argv[2])
L3_SIZE=2*L2_SIZE
#open file,read reuse distance
x1=[]
y1=[]
x2=[]
y2=[]
i=0
L1_RD_FILE = 'output/reuse_dist_1'
L2_RD_FILE = 'output/reuse_dist_2_'+sys.argv[2]
for line in open(L1_RD_FILE,'r'):
	i=i+1
	if(i<L3_SIZE):
		tmp = line.strip().split()
		x1.append(int(tmp[0]))
		y1.append(int(tmp[1]))
i=0
for line in open(L2_RD_FILE,'r'):
	i=i+1
	if(i<L3_SIZE):
		tmp = line.strip().split()
		x2.append(int(tmp[0]))
		y2.append(int(tmp[1]))
#initial plot 
fig = plt.figure()
ax = fig.add_subplot(111)
ax.autoscale(True)

l1, = ax.plot(x1,y1,'b')#L1 distance hist
l2, = ax.plot(x2,y2,'r')#L2 distance hist
l3, = ax.plot([L1_SIZE,L1_SIZE],[0, 2*np.mean(y2)], label='L1 size', color='green',linewidth=2)
l4, = ax.plot([L2_SIZE,L2_SIZE],[0, 2*np.mean(y2)], label='L2 size', color='orange',linewidth=2)
ax.axis([0, L3_SIZE, 0, 2 * np.mean(y2)])
plt.xlabel("Reuse Distance")
plt.ylabel("Memory Access")
plt.title("Histogram")
axcolor = 'lightgoldenrodyellow'
sv_access= 0
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
	l1.set_xdata(x1)
	l1.set_ydata(y1)	
	l2.set_xdata(x2)
	l2.set_ydata(y2)	
	ax.set_xlim([0, L3_SIZE])
	ax.set_ylim([0, 2*np.mean(y2[0:L3_SIZE])*(2**sv_access)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level1(event):
	global now_level
	now_level = 1
	l1.set_xdata(x1[0:L1_SIZE])
	l1.set_ydata(y1[0:L1_SIZE])	
	l2.set_xdata(x2[0:L1_SIZE])
	l2.set_ydata(y2[0:L1_SIZE])	
	ax.set_xlim([0, L1_SIZE])
	ax.set_ylim([0, 2*np.mean(y2[0:L1_SIZE])*(2**sv_access)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level2(event):
	global now_level
	now_level = 2 
	l1.set_xdata(x1[L1_SIZE:L2_SIZE])
	l1.set_ydata(y1[L1_SIZE:L2_SIZE])	
	l2.set_xdata(x2[L1_SIZE:L2_SIZE])
	l2.set_ydata(y2[L1_SIZE:L2_SIZE])	
	ax.set_xlim([L1_SIZE, L2_SIZE])
	ax.set_ylim([0, 2*np.mean(y2[L1_SIZE:L2_SIZE])*(2**sv_access)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
def click_level3(event):
	global now_level
	now_level = 3
	l1.set_xdata(x1[L2_SIZE:L3_SIZE])
	l1.set_ydata(y1[L2_SIZE:L3_SIZE])	
	l2.set_xdata(x2[L2_SIZE:L3_SIZE])
	l2.set_ydata(y2[L2_SIZE:L3_SIZE])	
	ax.set_xlim([L2_SIZE, L3_SIZE])
	ax.set_ylim([0, 2*np.mean(y2[L2_SIZE:L3_SIZE])*(2**sv_access)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
b_levelt.on_clicked(click_levelt)
b_level1.on_clicked(click_level1)
b_level2.on_clicked(click_level2)
b_level3.on_clicked(click_level3)


#Slider
ax_access  = plt.axes([0.25, 0.04, 0.05 , 0.03], axisbg=axcolor)
s_access  = Slider(ax_access, 'access', 0, 15, valinit=0,valfmt='%d')
def update(val):
	global sv_access
	sv_access = int(s_access.val)
	if(now_level == 0):
		ax.set_ylim([0, 2*np.mean(y2[0:L3_SIZE])*(2**sv_access)])		
	elif(now_level == 1):
		ax.set_ylim([0, 2*np.mean(y2[0:L1_SIZE])*(2**sv_access)])
	elif(now_level == 2):
		ax.set_ylim([0, 2*np.mean(y2[L1_SIZE:L2_SIZE])*(2**sv_access)])
	elif(now_level == 3):
		ax.set_ylim([0, 2*np.mean(y2[L2_SIZE:L3_SIZE])*(2**sv_access)])
	ax.relim()
	ax.autoscale_view()
	fig.canvas.draw_idle()
s_access.on_changed(update)

ax.legend()
plt.show()
