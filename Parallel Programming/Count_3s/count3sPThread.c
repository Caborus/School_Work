/* Alexander Cabos
 * CS 491
 * Chapter 6 - OpenMP and Java
 * paddingControl.c: Counts the number of 3s in an array using Pthreads
 * This program will take a command line argument for the array length
 * print out the number of 3s counted and the runtime in seconds, and
 * then record the time in a text file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// Definitions
#define MAX_THREADS 30

// Global variables
int *array;
int length;
int count;
int t = 30;
int range = 10;
pthread_mutex_t m;

struct padded_int
{
    int value;
    char padding[60];
} private_count[MAX_THREADS];

typedef struct thread_data
{
    int id;
    int start;
    int end;
} thread_data;

// Function prototypes
void *count3s_thread(void *arg);
void init(int range);
void count3s_parallel();

int main(int argc, char *argv[])
{
    // Get the array length from the command line
    if (argc != 2)
    {
        printf("Usage: ./paddedControl <length>\n");
        return 1;
    }

    // Set array length
    length = atoi(argv[1]);

    // Allocate memory for the array
    array = (int *)malloc(length * sizeof(int));

    // Initialize the array with random values
    init(range);

    // Start the timer
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Count the number of 3s in parallel
    count3s_parallel();

    // Stop the timer
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // Calculate the elapsed time
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Print the results
    printf("3s Count:    %d\n", count);
    printf("Runtime (s): %f\n", elapsed);

    // Add time to file "controlRuntimes.txt"
    FILE *f = fopen("paddedRuntimes.txt", "a");
    fprintf(f, "%d: %f\n", length, elapsed);
    fclose(f);

    // Free allocated memory
    free(array);

    return 0;
}

// Initialize array with random values
void init(int range)
{
    int i;
    srand(time(NULL));

    for (i = 0; i < length; i++)
    {
        array[i] = rand() % range;
    }
}

// Count the number of 3s in parallel
void count3s_parallel()
{
    // Initialize pthread variables
    pthread_t threads[t];
    pthread_mutex_init(&m, NULL);
    thread_data data[t];

    // Create the threads
    for (int i = 0; i < t; i++)
    {
        data[i].id = i;
        data[i].start = i * (length / t);
        data[i].end = (i + 1) * (length / t);

        pthread_create(&threads[i], NULL, count3s_thread, (void *)&data[i]);
    }

    // Join the threads
    for (int i = 0; i < t; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return;
}

// Count the number of 3s in a section of the array assigned to a thread
void *count3s_thread(void *arg)
{
    // Get the thread id
    struct thread_data *data = (struct thread_data *)arg;
    int id = data->id;
    int start = data->start;
    int end = data->end;

    // Initialize private count
    private_count[id].value = 0;

    // Count the number of 3s in the section of the array assigned to this thread
    for (int i = start; i < end; i++)
    {
        if (array[i] == 3)
        {
            private_count[id].value++;
        }
    }

    // Add the private count to the global count
    pthread_mutex_lock(&m);
    count += private_count[id].value;
    pthread_mutex_unlock(&m);

    return NULL;
}