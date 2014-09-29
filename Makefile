sim_cache: sim_main.o cache.o
	g++ sim_main.o cache.o -o sim_cache 
	mv sim_main.o cache.o obj
sim_main.o:
	g++ -c sim_main.cpp
cache.o:
	g++ -c cache.cpp

.PHONY: clean run
clean:
	rm obj/*.o sim_main
run:
	./sim_cache
