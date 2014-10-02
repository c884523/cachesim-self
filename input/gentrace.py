fp = open("seqstrace","w")
x=1
for i in range(1,1000000,1):
	fp.write('R '+str(hex(x*64))+'\n')	
	x+=1

