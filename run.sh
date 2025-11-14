#!/bin/bash
#SBATCH --ntasks=1000
#SBATCH --nodes=20
#SBATCH --time=02:00:00
#SBATCH --partition=cpu
#SBATCH --output=logs.json
#SBATCH --job-name=logging_test

# export environment variables
export UCX_TLS=self,sm,rc,ud
export OMPI_MCA_PML="ucx"
export OMPI_MCA_osc="ucx"

export OPENBLAS_NUM_THREADS=1
export GOTO_NUM_THREADS=1
export OMP_NUM_THREADS=1

# Run the solver
srun python3 main.py
#mpirun python3 main.py