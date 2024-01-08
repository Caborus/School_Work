/*
 * Alexander Cabos
 * CS 474
 * 10/15/2023
 * Programming 3: normal.c
 * Purpose: This program will request an input value from the command line and create an array of that size,
 *     filled with random integers. It will then sort the array using a sequential mergesort algorithm, and print
 *    the time taken to sort the array.
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

// Global variables to store array and size
int arraySize;
int *array;
int *out;

// Function prototypes
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

    // Allocate memory for array
    array = (int *)malloc(arraySize * sizeof(int));
    out = (int *)malloc(arraySize * sizeof(int));

    // Fill array with random integers
    fillArray(array, arraySize);

    // Print array size to be sorted
    printf("Array size: %d\n", arraySize);

    // Print unsorted array, uncomment to print
    // printf("Unsorted array: \n");
    // printArray(array, arraySize);

    // get start time
    if (clock_gettime(CLOCK_REALTIME, &s_starttimer) != 0)
    {
        printf("clock_gettime error\n");
        exit(1);
    }

    // Sort array
    sort(0, arraySize - 1);

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
    printf("Time taken to run normal mergesort: %lld nanoseconds\n", j);

    // Free memory
    free(array);
    free(out);

    return 0;
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
