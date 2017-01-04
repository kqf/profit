#PBS -k oe
#PBS -l nodes=1:ppn=30
#PBS -M petrov.knu@gmail.com
#PBS -m abe
#PBS -q long
#PBS -N nonlinear-trajectory
#!/bin/bash


cd /home/okovalen/nonlinear-trajectories
date  >> someoutput
source /home/okovalen/.configure_env.sh  &>> someoutput
./profit /home/okovalen/nonlinear-trajectories/ &>> someoutput
echo "Worked somehow" >> someoutput
date >> someoutput

echo "Your computation is done" | mail -s "My task" petrov.knu@gmail.com
