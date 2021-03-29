import datetime
import os
import subprocess

parallel_out = "output_parallel"
consistent_out = "output_parallel"

if __name__ == '__main__':
    print('hello')
    print(datetime.datetime.now())

    with open(parallel_out, 'w') as output:
        subprocess.call(['./parallel'], stdout=output)

    print(datetime.datetime.now())

    with open(consistent_out, 'w') as output:
        subprocess.call(['./consistent'], stdout=output)

    print(datetime.datetime.now())
