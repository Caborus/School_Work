/* Alexander Cabos
 * CS 491
 * Chapter 6 - OpenMP and Java
 * count3sOMP.c: Counts the number of 3s in an array using OpenMP
 * This program will take a command line argument for the array length
 * print out the number of 3s counted and the runtime in seconds, and
 * then record the time in a text file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Global variables
int *array;
int length;
int count;
int range = 10;

// Function prototypes
void init(int range);
int count3s();

int main(int argc, char *argv[])
{
    // Get the array length from the command line
    if (argc != 2)
    {
        printf("Usage: ./paddedControl <length>\n");
        return 1;
    }

    length = atoi(argv[1]);

    // Allocate memory for the array
    array = (int *)malloc(length * sizeof(int));

    // Initialize the array with random values
    init(range);

    // Start the timer
    double start = omp_get_wtime();

    // Count the number of 3s in parallel
    count = count3s();

    // Stop the timer
    double end = omp_get_wtime();
    double elapsed = end - start;

    // Print the results
    printf("3s Count:    %d\n", count);
    printf("Runtime (s): %f\n", elapsed);

    // Add time to file "controlRuntimes.txt"
    FILE *f = fopen("OMPRuntimes.txt", "a");
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

// Count the number of 3s in parallel using OpenMP and reduction with edge management
int count3s()
{
    // Initialize variables
    int i, count_p;
    count = 0;

    // Create a parallel region
#pragma omp parallel shared(array, count, length) private(count_p)
    {
        // Initialize private count
        count_p = 0;

        // Create a parallel for loop
#pragma omp parallel for private(i)
        for (i = 0; i < length; i++)
        {
            printf("%d\n", omp_get_thread_num());
            // Check if the current element is 3
            if (array[i] == 3)
            {
                // Increment the private count
                count_p++;
            }
        }

        // Create a critical section
#pragma omp critical
        {
            // Increment the global count
            count += count_p;
        }
    }

    // Return the global count
    return count;
}
