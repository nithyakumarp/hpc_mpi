#!/bin/bash

#SBATCH --nodes=1
#SBATCH --job-name=pi
#SBATCH --reservation=EECE5640
#SBATCH --tasks-per-node=2
#SBATCH --partition=short
#SBATCH --output=pi.%j.out

cd /scratch/$USER/GPUClassS19/HOL3/

set -o xtrace
./vadd 100000000
