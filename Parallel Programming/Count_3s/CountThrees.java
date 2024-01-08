/* Alexander Cabos
 * CS 491
 * Chapter 6 - OpenMP and Java
 * CountThrees.java: Counts number of threes in an array using Java threads
 * This program will take a command line argument for the array length
 * print out the number of 3s counted and the runtime in seconds, and 
 * then record the time in a text file.
*/

import java.util.*;
import java.util.concurrent.*;
import java.io.*;

public class CountThrees implements Runnable {
    private static final int MAX_THREADS = 30;
    private static final int MAX_RANGE = 10;
    private static final Random random = new Random();
    private static int count = 0;
    private static Object lock = new Object();
    private static int[] array;
    private static int arrayLength;
    private static Thread[] t;

    public static void main(String[] args) {
        // Get command line argument for array length
        if (args.length != 1) {
            System.out.println("Usage: java CountThrees <array length>");
            System.exit(0);
        }
        arrayLength = Integer.parseInt(args[0]);
        array = new int[arrayLength];

        for (int i = 0; i < arrayLength; i++) {
            array[i] = random.nextInt(MAX_RANGE);
        }

        long startTime = System.nanoTime();

        // create the threads
        CountThrees[] counters = new CountThrees[MAX_THREADS];
        int lengthPerThread = arrayLength / MAX_THREADS;
        t = new Thread[MAX_THREADS];

        for (int i = 0; i < counters.length; i++) {
            counters[i] = new CountThrees(i * lengthPerThread, lengthPerThread);
        }

        // run the threads
        for (int i = 0; i < counters.length; i++) {
            t[i] = new Thread(counters[i]);
            t[i].start();
        }
        for (int i = 0; i < counters.length; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                /* do nothing */
            }
        }

        long endTime = System.nanoTime();
        long elapsed = endTime - startTime;
        double elapsedSeconds = elapsed / 1000000000.0;

        // print the number of threes and the time taken in seconds
        System.out.println("3s Count:    " + count);
        System.out.printf("Runtime (s): %f\n", elapsedSeconds);

        // Add the array length and elapsedSeconds to "javaRuntimes.txt" in new line
        try {
            FileWriter fw = new FileWriter("javaRuntimes.txt", true);
            fw.write(arrayLength + ": " + elapsedSeconds + "\n");
            fw.close();
        } catch (IOException e) {
            System.out.println("Error writing to file");
        }
    }

    private int startIndex;
    private int elements;
    private int myCount = 0;

    public CountThrees(int start, int elem) {
        startIndex = start;
        elements = elem;
    }

    // Overload of run method in the Thread class
    public void run() {
        // count the number of threes
        for (int i = 0; i < elements; i++) {
            if (array[startIndex + i] == 3) {
                myCount++;
            }
        }
        synchronized (lock) {
            count += myCount;
        }
    }
}
