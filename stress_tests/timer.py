import datetime
import os
import subprocess
import time

parallel_out = "output_parallel"
consistent_out = "output_consistent"

if __name__ == '__main__':
    time_start_parallel = time.time()
    with open(parallel_out, 'w') as output:
        print('Parallel running...')
        subprocess.call(['./parallel'], stdout=output)
        print('Parallel end!')

    time_end_parallel = time.time()

    time_start_consistent = time.time()
    with open(consistent_out, 'w') as output:
        print('Consistent running...')
        subprocess.call(['./consistent'], stdout=output)
        print('Consistent end!')

    time_end_consistent = time.time()

    print("parallel:")
    print(time_end_parallel - time_start_parallel)
    print("consistent:")
    print(time_end_consistent - time_start_consistent)
