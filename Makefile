INT_BENCH ="/media/tsaicl/CPU2006_TEST/int"
FP_BENCH  ="/media/tsaicl/CPU2006_TEST/float"


sim_cache: sim_cache.o cache.o
	g++  -O3 sim_cache.o cache.o -o sim_cache 
	mv sim_cache.o cache.o obj
sim_cache.o:
	g++  -O3 -c sim_cache.cpp
cache.o:
	g++  -O3 -c cache.cpp
.PHONY: clean run perlbench bzip2 gcc mcf gobmk hmmer sjeng lib h264ref omnetpp astar xalancbmk
clean:
	rm obj/*.o sim_cache
