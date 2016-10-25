Amplitude analysis:  
=====================================

Performs minimization of `A(s, t)` amplitude function.

## Run ##
This par remains the same for all the machines.

```bash
!bash

./profit /full/path/to/your/data-and-parameters

```


## Build ##
Environtment is fine on `lxplus`. 
For BITP one should check if environment is properly set.


## Submit your job ##
Submit jobs to 2 weeks queue at `lxplus` with `16` threads:  
```
#!bash
bsub -q 2nw -n 16 -R "span[hosts=1]" nonlinear.sh
# check status
bjobs

# kill your jobs
bkill 123456

# kill all 
bkill -u username

```  


BITP cluster:

```bash
#!bash
# ssh -XYt xxx@clusterui.bitp.kiev.ua
# cd nonlinear-trajectories

qsub pnonlinear-trajectory.sh

# check status
qstat

# delete 
qdel 123456
```

## Warning(s)##
Remember to copy your `.root` files before submitting your jobs.











