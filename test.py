
import os
import subprocess

def frange(start, stop, step):
    i = start
    while i < stop:
        yield i
        i += step

def captureOutput(arrivalRate, serviceTime):
    args = ["./simulator" , str(arrivalRate), str(serviceTime), "7890", "all"]
    popen = subprocess.run(args, stdout=subprocess.PIPE)
    output = popen.stdout.decode("utf-8").split('\n')
    print(output)

for arrivalRate in range(0, 10, 1):
    for serviceTime in frange(0, 5.0, 0.5):
        captureOutput(arrivalRate, serviceTime)


