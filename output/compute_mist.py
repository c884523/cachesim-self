import sys


L1_SIZE=int(sys.argv[1])#L1 cache size,fully associate
TOTAL  =float(sys.argv[2])#total input numbers
i=0
hits=0.0
print(TOTAL)
for line in open('reuse_dist','r'):
	line = line.strip().split()
	if( i < L1_SIZE):
		hits += float(line[1])
	else:
		break
	i = i+1
print('hits = %d' % hits)
print('hit rate=%.3f' % (100*hits/TOTAL) )
print('miss rate=%.3f' % (100*(TOTAL-hits)/TOTAL) )

