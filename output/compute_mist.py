import sys


L1_SIZE=int(sys.argv[2])#L1 cache size,fully associate
L2_SIZE=int(sys.argv[3])
TOTAL  =float(sys.argv[4])#total input numbers
i=0
L1_hits=0.0
L2_hits=0.0
for line in open(sys.argv[1],'r'):
	line = line.strip().split()
	if( i < L1_SIZE):
		L1_hits += float(line[1])
	elif(i < L2_SIZE):
		L2_hits += float(line[1])
	else:
		break
	i = i+1
print('L1_hits = %d' % L1_hits)
print('L1_miss = %d' % (TOTAL-L1_hits))
print('L1 hit rate=%.3f' % (100*L1_hits/TOTAL) )
print('L1 miss rate=%.3f' % (100*(TOTAL-L1_hits)/TOTAL) )
print('==========================\n')
print('L2_hits = %d' % L2_hits)
print('L2_miss = %d' % (TOTAL-L1_hits-L2_hits))
#print('L2 hit rate=%.3f' % (100*L2_hits/TOTAL-) )
print('L2 miss rate=%.3f' % (100*(TOTAL-L2_hits-L1_hits)/TOTAL) )

