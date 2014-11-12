import sys


L1_SIZE=int(sys.argv[2])#L1 cache size,fully associate
L2_SIZE=int(sys.argv[3])
TOTAL  =float(sys.argv[4])#total input numbers
i=0
revised=0.0 #used to more accuracy, positive => should minus , negtive => should add
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
	if( i >= L2_SIZE-(L1_SIZE-1) and i <= L2_SIZE-1):
		revised += float(line[1]) * (L1_SIZE-(L2_SIZE-i))# have weight to multiply
	if( i >= L2_SIZE and i <= L2_SIZE+(L1_SIZE-1)):
		revised -= float(line[1]) * (L1_SIZE-(i-L2_SIZE+1))
	i = i+1
fp = open('sim_result_%d_%d' % (L1_SIZE,L2_SIZE) , 'r')
lines = fp.readlines()
print('=====L1 cache=====')
print('          |   Eval     |   Real   ')
print('hits      | %-10d | %s' % (L1_hits,lines[0]) ),
print('miss      | %-10d | %s' % (TOTAL-L1_hits,lines[1])),
print('hit rate  | %-10.3f | %s' % (100*L1_hits/TOTAL,lines[2]) ),
print('miss rate | %-10.3f | %s' % (100*(TOTAL-L1_hits)/TOTAL,lines[3]) ),
print('=====L2 cache=====')
print('hits      | %-10d | %s' % (L2_hits,lines[4])),
print('miss      | %-10d | %s' % (TOTAL-L1_hits-L2_hits,lines[5])),
print('hit rate  | %-10.3f | %s' % (100*L2_hits/(TOTAL-L1_hits),lines[6]) ),
print('miss rate | %-10.3f | %s' % (100*(TOTAL-L2_hits-L1_hits)/(TOTAL-L1_hits),lines[7]) ),
print('=====Error rate & Revised=====')
print('Before : E_hits = %d , E_hrate = %.3f' % ( L2_hits-int(lines[4]) , 100*L2_hits/(TOTAL-L1_hits)-float(lines[6])))
revised /= 2*L1_SIZE-1
print('Revised hits    = %d' % int(revised))
print('After  : E_hits = %d , E_hrate = %.3f' % ( L2_hits-int(revised)-int(lines[4]) , 100*(L2_hits-revised)/(TOTAL-L1_hits)-float(lines[6])))
