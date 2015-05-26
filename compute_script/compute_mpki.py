import sys


L1_SIZE=int(sys.argv[2])#L1 cache size,fully associate
L2_SIZE=int(sys.argv[3])#L2
L3_SIZE=int(sys.argv[4])#L3

L1_hits=0.0
L2_hits=0.0
L3_hits=0.0
i=0
total  =0.0
last=0.0
for line in open(sys.argv[1],'r'):
	line = line.strip().split()
	if( i < L1_SIZE):
		L1_hits += int(line[1])
	elif(i < L2_SIZE):
		L2_hits += int(line[1])
	elif(i < L3_SIZE):
		L3_hits += int(line[1])
	total += int(line[1])
	last   = int(line[1])
	i += 1
total -= last
print( total )
print( L3_hits/total )
