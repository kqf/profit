#!/bin/bash
#PBS -q long
#PBS -l nodes=1:ppn=30
#PBS -o /home/okovalen/test/
#PBS -j oe
#PBS -N nonlinear-trajectory

cd /home/okovalen/nonlinear-trajectories
date  >> someoutput
source /home/okovalen/.configure_env.sh  &>> someoutput
./profit /home/okovalen/nonlinear-trajectories/ &>> someoutput
echo "Worked somehow" >> someoutput
date >> someoutput

echo "Your computation is done" | mail -s "My task" petrov.knu@gmail.com
