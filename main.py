from mpi4py import MPI
import warnings
import sys

import test_module

async_logger = True
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
    total_time = df['time'].sum()

    try:
        with open('results.json', 'r+') as f:
                previus_results = json.load(f)
    except:
        previus_results = []
    
    with open('results.json', 'w') as f:
        result = {
            'async': async_logger,
            'start': data['start'],
            'end': data['end'],
            'total_time': total_time
        }
        previus_results.append(result)
        json.dump(previus_results, f)


if __name__ == "__main__":
    async_logger = sys.argv[1] != '0'
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    loops = 6000
    sleep_ms = 100
    test_module.run(rank, loops, sleep_ms, async_logger)
    if rank == 0:
        check_logs('logs.json')
