#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ext4.h"

int main(int argc, char *argv[])
{
    EXT4_SUPER_BLOCK MY_EXT4;

    FILE *fp;
    char *filename;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    filename = argv[1];

    // Open the file
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // use fseek to move forward 1024 bytes
    fseek(fp, 1024, SEEK_SET);

    // read in the struct from the file
    fread(&MY_EXT4, sizeof(MY_EXT4), 1, fp);

    // close the file
    fclose(fp);

    // print
    printf("INODES %x\n", MY_EXT4.s_inodes_count);
    printf("BLOCKS %x\n", MY_EXT4.s_blocks_count_lo);
    printf("MAGIC %x\n", MY_EXT4.s_magic);
    // print timestamp of last mount
    printf("Last mounted: %s\n", MY_EXT4.s_last_mounted);
    printf("volume Name: %s\n", MY_EXT4.s_volume_name);
    printf("Volume ID: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%x", MY_EXT4.s_uuid[i]);
    }
    printf("\n");

    return 0;
}