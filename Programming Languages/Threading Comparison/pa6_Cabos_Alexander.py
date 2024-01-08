## Alexander Cabos
## CS 471
## PA6 - Concurrency

import random
import time

## CREATES A MATRIX OF SIZE N AND FILLS WITH RANDOM INTEGERS FROM 0 - 2^20 ##
def createMatrix(n):
    newMatrix = [[random.randint(0, 2**20) for i in range(n)] for j in range(n)]
    return newMatrix


## CALCULATES MIN, MAX, AND AVERAGE WITHOUT THREADING ##
def calcNoThread(m):
    start_time = time.perf_counter()
    n = len(m)
    min = m[0][0]
    max = m[0][0]
    tot = 0
    avg = 0
    for i in range(0, n):
        for j in range(0, n):
            var = m[i][j]
            if min > var:
                min = var
            if max < var:
                max = var
            tot += var
    avg = tot / (n * n)

    print("Max value: ", max, "\nMin value: ", min, "\nAverage value: ", avg)
    print("Time taken to compute max, min, and average: ", (time.perf_counter() - start_time) * 10**3, " milliseconds")





## EXAMPLE OF CONCURRENCY ##

## MAIN FUNCTION THAT CALLS THE THREADS AND FINISHES THE FINAL CALCULATIONS ##
def calcWithThread(m):
    start_time = time.perf_counter()
    n = len(m)

    prev = thread(m[0])
    min = prev[0]
    max = prev[1]
    avg = prev[2]
    for i in range(1, n):
        curr = thread(m[i])
        if curr[0] < min:
            min = curr[0]
        if curr[1] > max:
            max = curr[1]
        avg += curr[2]
        prev = curr  
    avg = avg / n

    print("Max value: ", max, "\nMin value: ", min, "\nAverage value: ", avg)
    print("Time taken to compute max, min, and average: ", (time.perf_counter() - start_time) * 10**3, " milliseconds")


## CHILD FUNCTION OF calcWithThread USED TO CALCULATE THE MIN, MAX, AND AVG OF EACH LINE ##    
def thread(m):
    n = len(m)
    max = m[0]
    min = m[0]
    avg = 0
    for i in range(1, n):
        val = m[i]
        if (val > max):
            max = val
        if (val < min):
            min = val
        avg += val
    avg = avg/n
    return [min, max, avg]



#####################################################################################
## USER INPUT AND CALLS FOR CREATING MATRIX AND RUNNING WITH AND WITHOUT THREADING ##
#####################################################################################

userin = int(input("FOR N = "))
m = createMatrix(userin)

print("\nTask 2:")
calcNoThread(m)

print("\nTask 3:")
calcWithThread(m)