mpic++ MPI.cpp -o mpi -lm

for t in 2 4 8 12 16
do 
	mpiexec -mca btl self,tcp -np  $t  mpi
done