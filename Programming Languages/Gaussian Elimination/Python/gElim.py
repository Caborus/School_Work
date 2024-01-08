import random
import time

def gElim(A, x, N):
    for i in range(N):
        if A[i][i] == 0.0:
            print('Divide by zero! D:')
            exit()
        for j in range(i+1, N):
            alpha = A[j][i]/A[i][i]
            for k in range(N+1):
                A[j][k] = A[j][k] - alpha * A[i][k]
        x[N-1] = A[N-1][N] / A[N-1][N-1]
        for i in range(N-2, -1, -1):
            x[i] = A[i][N]
            for j in range(i+1, N):
                x[i] = x[i] - A[i][j] * x[j]
            x[i] = x[i] / A[i][i]

def main():
    N = int(input('Enter size of the Matrix (100, 500, 1000, 1500, 2000): '))
    start_time = time.time()
    A = []
    for i in range(N):
        col = []
        for j in range(N+1):
            col.append(random.randint(-100, 100))
        A.append(col)
    x = []
    for i in range(N):
        col = []
        for j in range(1):
            col.append(0.0)
        x.append(col)

    gElim(A, x, N)
    end_time = time.time()

    print ("Total time (s) taken by the program without numpy: ", end_time-start_time)

if __name__ == "__main__":
    main()