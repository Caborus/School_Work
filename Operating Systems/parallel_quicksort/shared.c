/*
 * Alexander Cabos
 * CS 474
 * 10/15/2023
 * Programming 3: shared.c
 * Purpose: This program will request an input value from the command line and create an array of that size,
 *    filled with random integers. It will then sort the array using a multiprocessed mergesort algorithm, and print
 *   the time taken to sort the array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_THREADS 8

// Global variables
int shared_key_in;
int shared_key_out;
int arraySize;
int *array;
int *out;
struct shmid_ds item;

// Function prototypes
void mergesort(int start, int end, int level);
void sort(int left, int right);
void merge(int loc1, int loc2, int loc3, int loc4);
void fillArray(int *array, int size);
void printArray(int *array, int size);

int main(int argc, char *argv[])
{
    // variables for timer
    long long j, k;
    struct timespec s_starttimer, s_endtimer;

    // Check for command line argument
    if (argc != 2)
    {
        // If no argument, request input from user
        printf("Please enter array size: ");
        scanf("%d", &arraySize);
    }
    else
    {
        // If argument provided, use that for arraySize
        arraySize = atoi(argv[1]);
    }

    // Define shared memory
    if ((shared_key_in = shmget(IPC_PRIVATE, arraySize * 4, IPC_CREAT | 0666)) < 0)
    {
        perror("Cannot get shared memory\n");
    }

    // Attach shared memory segment
    if ((array = shmat(shared_key_in, (void *)0, SHM_RND)) == (void *)-1)
    {
        perror("cannot attach to shared memory\n");
    }

    // Define shared memory
    if ((shared_key_out = shmget(IPC_PRIVATE, arraySize * 4, IPC_CREAT | 0666)) < 0)
    {
        perror("Cannot get shared memory\n");
    }

    // Attach shared memory segment
    if ((out = shmat(shared_key_out, (void *)0, SHM_RND)) == (void *)-1)
    {
        perror("cannot attach to shared memory\n");
    }

    // Fill array with random integers
    fillArray(array, arraySize);

    // Print array size to be sorted
    printf("Array size: %d", arraySize);
    printf("\n");

    // Print unsorted array, uncomment to print
    // printf("Unsorted array: \n");
    // printArray(array, arraySize);

    // get start time
    if (clock_gettime(CLOCK_REALTIME, &s_starttimer) != 0)
    {
        printf("clock_gettime error\n");
        exit(1);
    }

    mergesort(0, arraySize - 1, 3);

    // capture the end time
    if (clock_gettime(CLOCK_REALTIME, &s_endtimer) != 0)
    {
        printf("clock_gettime error\n");
        exit(1);
    }

    j = s_endtimer.tv_sec - s_starttimer.tv_sec;
    k = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
    j = j * 1000000000 + k;

    // Print sorted array, uncomment to print
    // printf("Sorted array: \n");
    // printArray(array, arraySize);

    // Print time taken to sort
    printf("Time taken to run shared mergesort: %lld nanoseconds\n", j);

    return 0;
}

// PRE: array to be sorted and size of array.
// POST: array is sorted in parallel.
void mergesort(int start, int end, int level)
{
    int mid;
    pid_t pid;
    int status;

    // Base case, run sort on array
    if (level <= 0)
    {
        sort(start, end);
    }
    // Recursive case, split array in half and run mergesort on each half, fork to solve right half
    else
    {
        // get midpoint
        mid = (start + end) / 2;

        // parent runs mergesort on left half
        mergesort(start, mid, level - 1);
        // fork to run mergesort on right half
        if ((pid = fork()) == 0)
        {
            // Attach shared memory for child
            if ((array = shmat(shared_key_in, (void *)0, SHM_RND)) == (void *)-1)
            {
                perror("cannot attach to shared memory\n");
            }

            if ((out = shmat(shared_key_out, (void *)0, SHM_RND)) == (void *)-1)
            {
                perror("cannot attach to shared memory\n");
            }

            mergesort(mid + 1, end, level - 1);
            exit(0);
        }
        // parent waits for child to finish then merges the two halves
        else
        {
            waitpid(pid, &status, 0);
            merge(start, mid, mid + 1, end);
        }
    }

    return;
}

// PRE: left and right bounds of array to be sorted.
// POST: array is sorted.
void sort(int left, int right)
{
    int mid;

    // base case
    if (left >= right)
    {
        return;
    }
    else
    {
        // Find midpoint
        mid = (left + right) / 2;
        // Sort left half
        sort(left, mid);
        // Sort right half
        sort(mid + 1, right);
        // Merge halves
        merge(left, mid, mid + 1, right);
    }
}

// PRE: left and right bounds of array to be merged.
// POST: array is merged.
void merge(int loc1, int loc2, int loc3, int loc4)
{
    int i, j, k, insert;
    i = loc1;
    j = loc3;
    insert = loc1;

    // merge the two segments
    while ((i <= loc2) && (j <= loc4))
    {
        // Compare values and insert into temporary array
        if (array[i] >= array[j])
        {
            out[insert++] = array[j++];
        }
        else
        {
            out[insert++] = array[i++];
        }
    }

    // Take care of any remainder in first segment
    for (k = i; k <= loc2; k++)
    {
        out[insert++] = array[k];
    }

    // Take care of any remainder in second segment
    for (k = j; k <= loc4; k++)
    {
        out[insert++] = array[k];
    }

    // Transfer merged data back to original array
    for (i = loc1; i <= loc4; i++)
    {
        array[i] = out[i];
    }

    return;
}

// PRE: size of the array to be filled.
// POST: array is filled with random integers.
void fillArray(int *array, int size)
{
    int i;

    // Seed random number generator
    srand(time(NULL));

    // Fill array with random integers
    for (i = 0; i < size; i++)
    {
        array[i] = rand() % 1000;
    }

    return;
}

// PRE: array to be printed and size of array.
// POST: array is printed to terminal in rows of 20.
void printArray(int *array, int size)
{
    int i;

    // Print array in rows of 20
    for (i = 0; i < size; i++)
    {
        printf("%3d ", array[i]);
        if ((i + 1) % 20 == 0)
        {
            printf("\n");
        }
    }

    printf("\n");

    return;
}
