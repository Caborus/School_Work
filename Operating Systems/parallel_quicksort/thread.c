/*
 * Alexander Cabos
 * CS 474
 * 10/15/2023
 * Programming 3: thread.c
 * Purpose: This program will request an input value from the command line and create an array of that size,
 *    filled with random integers. It will then sort the array using a threaded mergesort algorithm, and print
 *  the time taken to sort the array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 100000000
#define MAX_THREADS 8

// Global variables
int t = MAX_THREADS;
int arraySize;
int *array;
int *out;
pthread_mutex_t m;

// Structure to hold thread data
struct thread_data
{
    int start;
    int end;
    int level;
} priv_data[MAX_THREADS];

// Function prototypes
void mergesort(int start, int end, int level);
void *mergesort_thread(void *args);
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

    // Initialize mutex
    pthread_mutex_init(&m, NULL);

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

    // Call mergesort
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
    printf("Time taken to run threaded mergesort: %lld nanoseconds\n", j);

    // Free memory
    free(array);
    free(out);

    return 0;
}

// PRE: left and right bounds of array to be sorted, and level of recursion.
// POST: array is sorted in parallel.
void mergesort(int start, int end, int level)
{

    // Check if level is 0 or less, or if there are no threads available
    if (level <= 0)
    {
        sort(start, end);
        return;
    }
    else
    {
        // Get midpoint
        int mid = (start + end) / 2;

        // if there are threads available, create a thread for the right half of the array
        // and recursively call mergesort on the left half
        if (t <= 0)
        {
            mergesort(start, mid, level - 1);
            mergesort(mid + 1, end, level - 1);
        }
        // else call call mergesort_thread on the right half of the array
        // and recursively call mergesort on the left half
        else
        {
            // decrement thread count
            pthread_mutex_lock(&m);
            t--;
            pthread_mutex_unlock(&m);

            // Create thread
            pthread_t thread;
            priv_data[t].start = mid + 1;
            priv_data[t].end = end;
            priv_data[t].level = level - 1;
            int error = pthread_create(&thread, NULL, mergesort_thread, &priv_data[t]);
            if (error)
            {
                printf("Error creating thread: %d\n", error);
                exit(-1);
            }

            // Recursive call mergesort on left half
            mergesort(start, mid, level - 1);

            // Wait for thread to finish
            pthread_join(thread, NULL);

            // Increment thread count
            pthread_mutex_lock(&m);
            t++;
            pthread_mutex_unlock(&m);
        }

        // Merge the two halves
        merge(start, mid, mid + 1, end);
    }

    return;
}

// PRE: thread data, containing start, end, and level of array to be sorted.
// POST: calls mergesort for the thread
void *mergesort_thread(void *args)
{
    // Get thread data
    struct thread_data *data = (struct thread_data *)args;
    // Call mergesort
    mergesort(data->start, data->end, data->level);
    return NULL;
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

    // Copy remaining values from left half
    while (i <= loc2)
    {
        out[insert++] = array[i++];
    }

    // Copy remaining values from right half
    while (j <= loc4)
    {
        out[insert++] = array[j++];
    }

    // Copy values from temporary array to original array
    for (k = loc1; k <= loc4; k++)
    {
        array[k] = out[k];
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
