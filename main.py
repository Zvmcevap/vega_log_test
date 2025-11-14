from mpi4py import MPI
import sys

import test_module

def check_logs(filename: str):
    import json
    import pandas as pd
    with open(filename, 'r') as f:
        data = json.load(f)
    df = pd.DataFrame(data=data['logs'])
    print(df)
    total_time = df['time'].sum()
    print(f"{data["end"] = }")
    print(f"Total time = {float(total_time)}")
    

if __name__ == "__main__":
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    loops = int(sys.argv[1])
    sleep_ms = int(sys.argv[2])
    test_module.run(rank, loops, sleep_ms)
    if rank == 0:
        check_logs('logs.json')