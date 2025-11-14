from mpi4py import MPI
import warnings
import sys

import test_module

def check_logs(filename: str):
    import json
    import pandas as pd
    with open(filename, 'r') as f:
        try:
            data = json.load(f)
        except json.JSONDecodeError as e:
            warnings.warn(
                f"JSON parsing failed in {filename}: {e.msg} "
                f"(line {e.lineno}, column {e.colno})"
            )
            sys.exit(1)
            
    df = pd.DataFrame(data=data['logs'])
    print(df)
    total_time = df['time'].sum() - data["start"]["start_time"]
    print(f"{data["end"] = }")
    print(f"Total time recorded = {float(total_time)}")    

if __name__ == "__main__":
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    loops = 600
    sleep_ms = 10
    test_module.run(rank, loops, sleep_ms)
    if rank == 0:
        check_logs('logs.json')