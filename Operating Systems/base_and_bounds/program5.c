/*
 * Alexander Cabos
 * CS 474 - Operating Systems
 * 11/12/2023
 * Program 5: Base and Bounds
 *
 * Purpose: This program simulates base and bounds memory management using a linked list. It takes a user input
 *      for the ram size, and then takes the following commands:
 *          START:      creates a process with a specified name and size and adds it to the memory list
 *          STOP:       stops a process with a specified name and removes it from the memory list
 *          DUMP:       prints the base and bounds of all active processes
 *          DUMP_FREE:  prints the base and bounds of all free memory blocks
 *          LOC:        prints the physical address of a process's virtual address
 *          END:        ends the simulation
 *
 * Design:
 *      For managing the memory, I use a single linked list and an int for tracking the total amount of free memory.
 *      The linked list holds both free spaces and active processes to make sure that the order of the list is
 *      maintained easily. At first run, the list gets initialized with a single free block that takes up the entire
 *      memory space, this allows me to keep track of each memory location.
 * START:
 *      When a process is started, the program checks if there is a free block big enough for the process. If there
 *      is, the process either replaces the free block or is inserted before the block and the free block gets adjusted.
 *      We garbage collect if we can't find a spot large enough for the process but there is enough free space.
 * STOP:
 *      When a process is stopped, the program looks through the list for the process, and updates the empty flag for
 *      that process to 1 to indicate that it's free. We also complete our compaction here, since I figured it made the
 *      most sense that way. The program just checks adjacent nodes for other free blocks, and if it finds one, it
 *      combines them.
 * GARBAGE COLLECTION:
 *      This brings us to the garbage collection strategy. Since we're using a linked list and tracking the entirety of
 *      memory, the program goes through the list and each time it finds a free block, it updates a counter for the
 *      amount of space we're moving, and removes that block from the list. And whenever we find a an active process,
 *      we update the base by subtracting the amount of space we've moved to that point. Once we get to the end of the
 *      list, all of the free blocks have been removed, so the counter now has the total of all of the free blocks, and
 *      we create a new free block at the end of the list with the size of the counter. Once that's all done, we call
 *      START again to add the new process to the end of the list of active processes.
 *
 *
 * Conclusion:
 *      Although I'm sure there are better ways to perform base and bounds memory management, the way I went made the
 *      most sense to me conceptually. I probably could've made it easier by using some arrays, but I wanted to get
 *      some linked list practice in and I think my method works pretty well, even if it is messy. This assignment
 *      definitely made me appreciate the simplicity of paging compared to this.
 *
 *
 * P.S: I hope I never have to do this again, this was a nightmare, I got so many seg faults and dumb errors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// struct for memory node
struct node
{
    char name[10];     // name of process
    int base;          // base of memory segment
    int size;          // size of memory segment
    int start;         // start time
    int end;           // end time
    int empty;         // 1 if empty, 0 if not
    struct node *next; // next node
};                     // end struct node

// Global Variables
int DEBUG = 0; // Debug flag

typedef struct node node;
node *head = NULL;    // head of memory linked list
int RAM = 0;          // size of RAM
int time = 0;         // current time
int numProcesses = 0; // number of processes
int numFree = 0;      // number of free blocks
int freeSpace = 0;    // total amount of free space

// Function Prototypes
void init();
int parse(char *buff, char *tokens[]);
void start(int startTime, char pName[10], int pSize);
void stop(char pName[10]);
void dump();
void dump_free();
void loc(char pName[10], int pAddr);
void compact(node *n1, node *n2);
void garbage_collect();
void printlist();

// Main Function
int main(int argc, char *argv[])
{
    int args;         // number of arguments
    char buff[100];   // buffer for input line
    char *tokens[10]; // array of arguments
    char c;           // character for input

    // check for debug flag '-d'
    if (argc == 2)
    {
        if (strcmp(argv[1], "-d") == 0)
        {
            DEBUG = 1;
        }
    }

    printf("\nBase and Bounds\n--------------------------------\n\n");
    // get RAM size
    while (1)
    {
        // clear buffers to try and avoid errors
        memset(buff, 0, sizeof(buff));
        memset(tokens, 0, sizeof(tokens));
        memset(&c, 0, sizeof(c));
        fflush(stdout);

        // read in the line
        scanf("%199[^\n]", buff);
        c = getchar();

        // parse the line
        args = parse(buff, tokens);

        // if ram size is given
        if (strcasecmp(tokens[0], "RAM") == 0)
        {
            // check if ram size is valid
            if (atoi(tokens[1]) > 0)
            {
                RAM = atoi(tokens[1]); // set ram size
                break;                 // break loop
            }
            else
            {
                printf("\nInvalid RAM size\n"); // print error message
            }
        }
    }

    // initialize memory linked list
    init();

    // clear buffers to try and avoid errors
    memset(buff, 0, sizeof(buff));
    memset(tokens, 0, sizeof(tokens));
    memset(&c, 0, sizeof(c));

    // start simulation and loop until END
    while (1)
    {
        fflush(stdout);

        // read in the line
        scanf("%199[^\n]", buff);
        c = getchar();

        // parse the line
        args = parse(buff, tokens);

        // Update time
        time = atoi(tokens[0]);

        if (DEBUG)
        {
            printf("\nTime: %d\n------------------------------\n", time);
        }

        // END
        if (strcasecmp(tokens[1], "END") == 0)
        {
            if (DEBUG)
                printf("\nEND SIMULATION\n");
            exit(0);
        }

        // START
        else if (strcasecmp(tokens[1], "START") == 0)
        {
            // check for correct number of arguments
            if (args != 4)
            {
                printf("\nInvalid number of arguments for START\n");
            }
            else
            {
                start(atoi(tokens[0]), tokens[2], atoi(tokens[3]));
            }
        }

        // STOP
        else if (strcasecmp(tokens[1], "STOP") == 0)
        {
            // check for correct number of arguments
            if (args != 3)
            {
                printf("\nInvalid number of arguments for STOP\n");
            }
            else
            {
                stop(tokens[2]); // stop process
            }
        }

        // DUMP
        else if (strcasecmp(tokens[1], "DUMP") == 0)
        {
            // check for correct number of arguments
            if (args != 2)
            {
                printf("\nInvalid number of arguments for DUMP\n");
            }
            else
            {
                dump(); // dump active processes
            }
        }

        // DUMP_FREE
        else if (strcasecmp(tokens[1], "DUMP_FREE") == 0)
        {
            // check for correct number of arguments
            if (args != 2)
            {
                printf("\nInvalid number of arguments for DUMP_FREE\n");
            }
            else
            {
                dump_free(); // dump free memory blocks
            }
        }

        // LOC
        else if (strcasecmp(tokens[1], "LOC") == 0)
        {
            // check for correct number of arguments
            if (args != 4)
            {
                printf("\nInvalid number of arguments for LOC\n");
            }
            else
            {
                loc(tokens[2], atoi(tokens[3])); // print physical address of process's virtual address
            }
        }

        // INVALID COMMAND
        else
        {
            printf("\nInvalid command\n");
        }

        memset(buff, 0, sizeof(buff));     // clear buffer
        memset(tokens, 0, sizeof(tokens)); // clear tokens
        memset(&c, 0, sizeof(c));          // clear c
    }

    fflush(stdout);
    return 0;
} // end main

// PRE: none
// POST: initializes memory linked list and sets free space tracker
void init()
{
    // create initial free block
    node *free = malloc(sizeof(node));
    free->base = 0;
    free->size = RAM;
    free->start = -1;
    free->end = -1;
    free->empty = 1;
    free->next = NULL;
    head = free;

    freeSpace = RAM; // set free space to ram size
    numFree = 1;     // set number of free blocks to 1

    if (DEBUG)
    {
        printf("\nCreated Initial free memory node\n");
        printlist();
    }

    fflush(stdout);
    return;
} // end init

// PRE: pointers to buffer and arguments
// POST: tokenizes buffer and returns number of arguments
int parse(char *buff, char *tokens[])
{
    int j;
    char *point;

    j = 0;

    // parse the buffer into tokens using strtok
    point = strtok(buff, "  ");

    // while there are still tokens
    while (point != NULL)
    {

        // store the token in tokens
        tokens[j] = point;

        // increment the number of arguments
        j++;

        // get the next token
        point = strtok(NULL, "     ");
    }

    // add a null terminator to the end of tokens
    tokens[j + 1] = '\0';

    // return the number of arguments
    fflush(stdout);
    return (j);
} // end parse

// PRE: start time, name, and size of process
// POST: adds process to memory linked list, garbage collect if necessary
void start(int startTime, char pName[10], int pSize)
{
    int found = 0;
    node *temp = head;
    node *prev = NULL;

    // check if given values are valid
    if (startTime < 0 || pSize <= 0)
    {
        printf("\nInvalid arguments for START\n");
        fflush(stdout);
        return;
    }

    // check if process already exists
    while (temp != NULL)
    {

        if (strcmp(temp->name, pName) == 0)
        {
            printf("\nProcess %s already exists\n", pName);
            fflush(stdout);
            return;
        }
        temp = temp->next;
    }

    // check if there is enough memory for process
    if (pSize > freeSpace)
    {
        printf("\nNot enough memory for process %s\n", pName);
        fflush(stdout);
        return;
    }

    // check if there is a free block big enough for process
    temp = head;
    while (temp != NULL)
    {
        // check for empty flag and compare size
        if (temp->empty == 1 && temp->size >= pSize)
        {
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // if there is a free block big enough for process
    if (found)
    {
        // compare size of free block to size of process
        if (temp->size == pSize)
        {
            // replace free block with process
            strcpy(temp->name, pName);
            temp->start = startTime;
            temp->end = -1;
            temp->empty = 0;

            numProcesses++;
            numFree--;
            freeSpace -= pSize;

            if (DEBUG)
            {
                printf("\nReplaced free block with process %s\n", pName);
            }
        }
        else
        {
            // create new node for process
            node *new = malloc(sizeof(node));
            strcpy(new->name, pName);
            new->base = temp->base;
            new->size = pSize;
            new->start = startTime;
            new->end = -1;
            new->empty = 0;
            new->next = temp;

            // adjust free block
            temp->base += pSize;
            temp->size -= pSize;

            numProcesses++;
            freeSpace -= pSize;

            // insert new node into linked list
            if (prev == NULL)
            {
                head = new;
            }
            else
            {
                prev->next = new;
            }

            if (DEBUG)
            {
                printf("\nInserted process %s into free block\n", pName);
                printlist();
            }
        }

        fflush(stdout);
        return;
    }

    // else no free space big enough
    else
    {
        // garbage collect
        garbage_collect();

        // call start again
        start(startTime, pName, pSize);
    }

    fflush(stdout);
    return;
} // end start

// PRE: name of process
// POST: replaces process with free block and combines adjacent free blocks
void stop(char pName[10])
{
    int found = 0;
    node *temp = head;
    node *prev = NULL;

    // check if process exists
    while (temp != NULL)
    {
        if (strcmp(temp->name, pName) == 0)
        {
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // if process exists
    if (found)
    {
        // check if process is already stopped
        if (temp->end != -1)
        {
            printf("\nProcess %s is already stopped\n", pName);
            fflush(stdout);
            return;
        }

        // replace process with free block
        strcpy(temp->name, "");
        temp->start = -1;
        temp->end = -1;
        temp->empty = 1;

        numProcesses--;
        numFree++;
        freeSpace += temp->size;

        // check if previous node is also free
        if (prev != NULL && prev->empty == 1)
        {
            // combine free blocks
            compact(prev, temp);
        }

        // check if next node is also free
        prev = temp;
        temp = temp->next;
        if (temp != NULL && temp->empty == 1)
        {
            // combine free blocks
            compact(prev, temp);
        }

        if (DEBUG)
        {
            printf("\nReplaced process %s with free block\n", pName);
        }
    }

    // else process does not exist
    else
    {
        printf("\nProcess %s does not exist\n", pName);
    }

    if (DEBUG)
    {
        printlist();
    }

    fflush(stdout);
    return;
} // end stop

// PRE: none
// POST: prints base and bounds of active processes
void dump()
{
    node *temp = head;

    printf("\n%d Active processes:\n", time);
    // loop through linked list
    while (temp != NULL)
    {
        // check if process is active
        if (temp->empty == 0)
        {
            // print base and bounds
            printf("%d | %s: base %d, bounds %d\n", time, temp->name, temp->base, temp->base + temp->size - 1);
        }
        temp = temp->next;
    }

    printf("\n");
    fflush(stdout);
    return;
} // end dump

// PRE: none
// POST: prints base and bounds of free memory blocks
void dump_free()
{
    node *temp = head;
    int i = 0;

    printf("\n%d Free blocks:\n", time);
    // loop through linked list
    while (temp != NULL)
    {
        // check if block is free
        if (temp->empty == 1)
        {
            // print base and bounds
            printf("%d | %d: base %d, bounds %d\n", time, i, temp->base, temp->base + temp->size - 1);
            i++;
        }
        temp = temp->next;
    }

    printf("\n");
    fflush(stdout);
    return;
} // end dump_free

// PRE: name of process and virtual address
// POST: prints physical address of process's virtual address
void loc(char pName[10], int pAddr)
{
    int found = 0;
    node *temp = head;

    // check if process exists
    while (temp != NULL)
    {
        if (strcmp(temp->name, pName) == 0)
        {
            found = 1;
            break;
        }
        temp = temp->next;
    }

    // if process exists
    if (found)
    {
        // check if virtual address is valid
        if (pAddr < 0 || pAddr >= temp->size)
        {
            printf("\nInvalid virtual address\n");
            fflush(stdout);
            return;
        }

        // print physical address
        printf("\n%d | %s: VIRTUAL ADDR %d -> PHYSICAL ADDR %d\n\n", time, temp->name, pAddr, temp->base + pAddr);
    }

    // else process does not exist
    else
    {
        printf("\nProcess %s does not exist\n", pName);
    }

    fflush(stdout);
    return;
} // end loc

// PRE: none
// POST: garbage collection: removes all free blocks, adjusts base of processes, creates singular free block at tail
void garbage_collect()
{
    node *temp = head;
    node *prev = NULL;
    int movedSpace = 0;

    printf("\n%d GARBAGE COLLECTION\n", time);
    // scan linked list for free blocks
    while (temp != NULL)
    {
        // check if block is free
        if (temp->empty == 1)
        {
            // check if block is at head
            if (prev == NULL)
            {
                // remove free block
                head = temp->next;
                movedSpace += temp->size;
                free(temp);
                temp = head;

                numFree--;

                if (DEBUG)
                {
                    printf("\nRemoved free block\n");
                }

                continue;
            }

            // else the block is not at the head
            else
            {
                // remove free block
                prev->next = temp->next;
                movedSpace += temp->size;
                free(temp);
                temp = prev->next;

                numFree--;

                if (DEBUG)
                {
                    printf("\nRemoved free block\n");
                }

                continue;
            }
        }

        // else block is not free
        else
        {
            // adjust base of process
            temp->base -= movedSpace;
            prev = temp;
            temp = temp->next;

            if (DEBUG)
            {
                printf("\nAdjusted base of process %s to %d\n", prev->name, prev->base);
            }
        }
    }

    // create a free block at the end of the linked list
    node *free = malloc(sizeof(node));
    free->base = RAM - movedSpace;
    free->size = movedSpace;
    free->start = -1;
    free->end = -1;
    free->empty = 1;
    free->next = NULL;

    // insert free block at end of list
    temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = free;

    numFree++;

    if (DEBUG)
    {
        printf("\nCreated free block at end of list\n");
    }

    fflush(stdout);
    return;
} // end garbage_collect

// PRE: two free block nodes
// POST: combines free blocks
void compact(node *n1, node *n2)
{
    // combine free blocks
    n1->size += n2->size;
    n1->next = n2->next;

    numFree--;

    if (DEBUG)
    {
        printf("\nCombined free blocks\n");
    }

    fflush(stdout);
    return;
} // end compact

// PRE: none
// POST: prints linked list
void printlist()
{
    node *temp = head;
    printf("\n--------------------\n#of processes: %d\n#of free blocks: %d\nFree space: %d\n", numProcesses, numFree, freeSpace);
    while (temp != NULL)
    {
        printf("Name: %s, Base: %d, Size: %d, Start: %d, End: %d, Empty: %d\n", temp->name, temp->base, temp->size, temp->start, temp->end, temp->empty);
        temp = temp->next;
    }
    printf("--------------------\n");
    fflush(stdout);
    return;
} // end printlist
