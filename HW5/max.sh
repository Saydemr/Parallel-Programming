
for t in 128
do
    for s in 10485760
    do
        nvcc -DNUMTRIALS=$s -DBLOCKSIZE=$t -o maxMul max.cu
        ./maxMul
    done
    echo
done