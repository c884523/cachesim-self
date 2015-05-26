import sys


L1_SIZE=int(sys.argv[2])#L1 cache size,fully associate
L2_SIZE=int(sys.argv[3])
L3_SIZE=int(sys.argv[4])
#TOTAL  =float(sys.argv[4])#total input numbers
i=0
revised_L2=0.0 #used to more accuracy, positive => should minus , negtive => should add
revised_L3=0.0 #used to more accuracy, positive => should minus , negtive => should add
L1_hits=0.0
L2_hits=0.0
L3_hits=0.0
for line in open(sys.argv[1],'r'):
	line = line.strip().split()
	if( i < L1_SIZE):
		L1_hits += float(line[1])
	elif(i < L2_SIZE):
		L2_hits += float(line[1])
	elif(i < L3_SIZE):
		L3_hits += float(line[1])
	else:
		break
	if( i >= L2_SIZE-(L1_SIZE-1) and i <= L2_SIZE-1):
		revised_L2 += float(line[1]) * (L1_SIZE-(L2_SIZE-i))# have weight to multiply
	if( i >= L3_SIZE-(L2_SIZE-1) and i <= L3_SIZE-1):
		revised_L3 += float(line[1]) * (L2_SIZE-(L3_SIZE-i))
	i = i+1
fp = open('sim_result_%d_%d_%d' % (L1_SIZE,L2_SIZE,L3_SIZE) , 'r')
lines = fp.readlines()
TOTAL = int(lines[0])+int(lines[1])
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
print('=====L3 cache=====')
print('hits      | %-10d | %s' % (L3_hits,lines[8])),
print('miss      | %-10d | %s' % (TOTAL-L1_hits-L2_hits-L3_hits,lines[9])),
print('hit rate  | %-10.3f | %s' % (100*L3_hits/(TOTAL-L1_hits-L2_hits),lines[10]) ),
print('miss rate | %-10.3f | %s' % (100*(TOTAL-L3_hits-L2_hits-L1_hits)/(TOTAL-L1_hits-L2_hits),lines[11]) ),
print('=====Error rate & Revised=====')
print('L2 : E_hits = %d , E_hrate = %.3f' % ( L2_hits-int(lines[4]) , 100*L2_hits/(TOTAL-L1_hits)-float(lines[6])))
print('L3 : E_hits = %d , E_hrate = %.3f' % ( L3_hits-int(lines[8]) , 100*L3_hits/(TOTAL-L1_hits-L2_hits)-float(lines[10])))
revised_L2 /= 4*L1_SIZE
revised_L3 /= 4*L2_SIZE
revised_L3 -= revised_L2
print('L2 Revised hits    = %d' % int(revised_L2))
print('After  : E_hits = %d , E_hrate = %.3f' % ( L2_hits-int(revised_L2)-int(lines[4]) , 100*(L2_hits-revised_L2)/(TOTAL-L1_hits)-float(lines[6])))
print('L3 Revised hits    = %d' % int(revised_L3))
print('After  : E_hits = %d , E_hrate = %.3f' % ( L3_hits-int(revised_L3)-int(lines[8]) , 100*(L3_hits-revised_L3)/(TOTAL-L1_hits-L2_hits+revised_L2)-float(lines[10])))
