./sim_cache input/my_tmp 32 1 64 1
./computing_RD L2_trace_64 L2 64
mv L2_trace_64 reuse_dist_2_64 output

./sim_cache input/my_tmp 64 1 128 1
./computing_RD L2_trace_128 L2 128
mv L2_trace_128 reuse_dist_2_128 output

./sim_cache input/my_tmp 128 1 256 1
./computing_RD L2_trace_256 L2 256
mv L2_trace_256 reuse_dist_2_256 output

./sim_cache input/my_tmp 256 1 512 1
./computing_RD L2_trace_512 L2 512
mv L2_trace_512 reuse_dist_2_512 output

./sim_cache input/my_tmp 512 1 1024 1
./computing_RD L2_trace_1024 L2 1024
mv L2_trace_1024 reuse_dist_2_1024 output
