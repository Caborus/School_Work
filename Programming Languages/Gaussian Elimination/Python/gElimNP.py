import numpy
from numpy import zeros
import time
import scipy


def gElim(A):
    P, L, U = scipy.linalg.lu(A)

def main():
    N = int(input('Enter size of the Matrix (100, 500, 1000, 1500, 2000): '))
    start_time = time.time()
    A = numpy.random.randint(-100, 100, size=(N, N+1))
    gElim(A)
    end_time = time.time()
    print("Total time (s) taken by the program with numpy: ", end_time - start_time)

if __name__ == "__main__":
    main()