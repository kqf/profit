START="$(date)"
NPROC="$(nproc)"

WORKDIR=/mnt/home/okovalenko/profit

cd $WORKDIR
module load root
make
./profit $WORKDIR/


echo "Done!\n\n\nCores: ${NPROC}\nStarted: ${START}\nStopped: $(date)" | mail -s "Nonlinear task (NCNR)" petrov.knu@gmail.com

