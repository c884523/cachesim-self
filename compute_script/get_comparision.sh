REUSE_SOURCE=/media/tsaicl/CPU2006_TEST/int

echo "*****perbench*****"
cd 1.perlbench
python ../compute_mist.py $REUSE_SOURCE/400.perlbench/perlbench_reuse_dist 16 64 2048

echo "*****bzip2*****"
cd ../2.bzip2
python ../compute_mist.py $REUSE_SOURCE/401.bzip2/bzip2_reuse_dist 16 64 2048

echo "*****gcc*****"
cd ../3.gcc
python ../compute_mist.py $REUSE_SOURCE/403.gcc/gcc_reuse_dist 16 64 2048

#echo "*****mcf*****"
#cd ../4.mcf
#python ../compute_mist.py $REUSE_SOURCE/429.mcf/mcf_reuse_dist 16 64 2048

echo "****gobmk*****"
cd ../5.gobmk
python ../compute_mist.py $REUSE_SOURCE/445.gobmk/gobmk_reuse_dist 16 64 2048

echo "****hmmer*****"
cd ../6.hmmer
python ../compute_mist.py $REUSE_SOURCE/456.hmmer/hmmer_reuse_dist 16 64 2048

echo "****sjeng*****"
cd ../7.sjeng
python ../compute_mist.py $REUSE_SOURCE/458.sjeng/sjeng_reuse_dist 16 64 2048

echo "****libquantum*****"
cd ../8.libquantum
python ../compute_mist.py $REUSE_SOURCE/462.libquantum/libquantum_reuse_dist 16 64 2048

echo "****h264ref*****"
cd ../9.h264ref
python ../compute_mist.py $REUSE_SOURCE/464.h264ref/h264ref_reuse_dist 16 64 2048

echo "****omnetpp*****"
cd ../10.omnetpp
python ../compute_mist.py $REUSE_SOURCE/471.omnetpp/omnetpp_reuse_dist 16 64 2048

echo "****astar*****"
cd ../11.astar
python ../compute_mist.py $REUSE_SOURCE/473.astar/astar_reuse_dist 16 64 2048

echo "****xalancbmk*****"
cd ../12.xalancbmk
python ../compute_mist.py $REUSE_SOURCE/483.xalancbmk/xalancbmk_reuse_dist 16 64 2048
