g++ proj4.cpp -o proj4 -lm -fopenmp

echo "Size,SimdMulSum,MulSum,SimdMul,Mul,,Speedup MulSum, Speedup Mul"

foreach ( $n in 1024, 10240, 16000, 32000, 64000, 128000, 256000, 512000, 1000000, 2000000, 8000000)
{
	echo $n "," 
	./proj4 $n
}