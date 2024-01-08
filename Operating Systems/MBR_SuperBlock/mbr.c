#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "mbr.h"

int main(int argc, char *argv[])
{
    MASTER_BOOT_RECORD MY_MBR;

    FILE *fp;
    char *filename;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    filename = argv[1];

    // Get the structure of the file
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Read the file into the structure
    fread(&MY_MBR, sizeof(MY_MBR), 1, fp);

    // Close the file
    fclose(fp);

    // Print the file name
    printf("File name: %s\n", filename);
    // print mbr signature
    printf("Signature: %x\n", MY_MBR.Signature);

    return 0;
}