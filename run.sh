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

# Run the solver
mpirun python3 main.py 1