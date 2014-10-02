INT_BENCH ="/media/tsaicl/CPU2006_TEST/int"
FP_BENCH  ="/media/tsaicl/CPU2006_TEST/float"



sim_cache: sim_main.o cache.o
	g++ sim_main.o cache.o -o sim_cache 
	mv sim_main.o cache.o obj
sim_main.o:
	g++ -DL1_W=$(a1) -DL2_W=$(a2) -DSET_L2=$(a3) -c sim_main.cpp
cache.o:
	g++ -c cache.cpp
.PHONY: clean run perlbench bzip2 gcc mcf gobmk hmmer sjeng lib h264ref omnetpp astar xalancbmk
clean:
	rm obj/*.o sim_cache
run:
	./sim_cache input/tmp
perlbench:
	./sim_cache $(INT_BENCH)/400.perlbench/all_trace
bzip2:
	./sim_cache $(INT_BENCH)/401.bzip2/all_trace
gcc:
	./sim_cache $(INT_BENCH)/403.gcc/all_trace
mcf:
	./sim_cache $(INT_BENCH)/429.mcf/all_trace
gobmk:
	./sim_cache $(INT_BENCH)/445.gobmk/all_trace
hmmer:
	./sim_cache $(INT_BENCH)/456.hmmer/all_trace
sjeng:
	./sim_cache $(INT_BENCH)/458.sjeng/all_trace
lib:
	./sim_cache $(INT_BENCH)/462.libquantum/all_trace
h264ref:
	./sim_cache $(INT_BENCH)/464.h264ref/all_trace
omnetpp:
	./sim_cache $(INT_BENCH)/471.omnetpp/all_trace
astar:
	./sim_cache $(INT_BENCH)/473.astar/all_trace
xalancbmk:
	./sim_cache $(INT_BENCH)/483.xalancbmk/all_trace
	
