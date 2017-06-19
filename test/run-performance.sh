NPROC_MIN=1
NPROC_MAX=80
NITER_MAX=1

for i in $(seq $NPROC_MIN $NPROC_MAX)
do
	# Use this loop to check statistical error of the result
	for dummy in $(seq 1 $NITER_MAX)
	do
		# echo $i $dummy
		mpirun -n $i ./test_amplitude_mpi.test
	done	
done