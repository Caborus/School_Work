/*
 * Alexander Cabos
 * CS 474: Operating Systems
 * Program 6: Programming Disks and Filesystems
 * Purpose: This program will duplicate the functionality of the Unix 'file' utility command.
 * It will take in a filesystem image and state if it's an MBR, Linux or unknown file type.
 * If it's an NTFS or Ext4 filesystem, it will state that and print out the appropriate
 * file information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "mbr.h"
#include "ext4.h"
#include "ntfs.h"

// Function prototypes
void printNTFS(NTFS_LAYOUT MY_NTFS);
void printEXT4(EXT4_SUPER_BLOCK MY_EXT4);

int main(int argc, char *argv[])
{
    MASTER_BOOT_RECORD MY_MBR;
    NTFS_LAYOUT MY_NTFS;
    EXT4_SUPER_BLOCK MY_EXT4;

    // Check for correct number of arguments
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    // Get the file name
    char *filename = argv[1];

    // Check if the file exists
    if (access(filename, F_OK) == -1)
    {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Read in file
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    // Read the mbr into the structure
    fread(&MY_MBR, sizeof(MY_MBR), 1, fp);

    // Read in the NTFS structure
    fseek(fp, 0, SEEK_SET);
    fread(&MY_NTFS, sizeof(MY_NTFS), 1, fp);

    // Read in the ext4 structure
    fseek(fp, 1024, SEEK_SET);
    fread(&MY_EXT4, sizeof(MY_EXT4), 1, fp);

    // Close the file
    fclose(fp);

    // Check if the file is an MBR
    if (MY_MBR.Signature == MBR_SIGNATURE)
    {
        printf("File type: MBR\n");
        printf("MBR signature: %x\n", MY_MBR.Signature);

        // Make sure this is a NTFS
        if (MY_NTFS.os[0] == 0x80)
        {
            printNTFS(MY_NTFS);
        }
        // If it's not an NTFS, it's unknown
        else
        {
            printf("unknown file subtype\n");
        }
    }
    // Check if the file is an ext4
    else if (MY_EXT4.s_magic == EXT4_SUPER_MAGIC)
    {
        printf("File type: Linux\n");
        printEXT4(MY_EXT4);
    }
    // At this point, we definitely don't know what it is
    else
    {
        printf("unknown file type\n");
    }

    return 0;
}

// PRE: An NTFS_LAYOUT struct
// POST: Prints the information about the NTFS filesystem
void printNTFS(NTFS_LAYOUT MY_NTFS)
{
    printf("Subtype: NTFS\n");
    printf("sector_size is 0x%x\n", MY_NTFS.sector_size);
    printf("ntfs_sectors_per_track is 0x%x\n", MY_NTFS.sectors_per_track);
    printf("ntfs_heads is 0x%x\n", MY_NTFS.number_of_heads);
    printf("ntfs_OS is 0x");
    for (int i = 0; i < 4; i++)
    {
        printf("%02x", MY_NTFS.os[i]);
    }
    printf("\n");
    printf("ntfs_total_sectors is 0x%llx == %lld\n", MY_NTFS.total_sectors, MY_NTFS.total_sectors);
    printf("ntfs_starting_cluster is 0x%llx == %lld\n", MY_NTFS.starting_cluster, MY_NTFS.starting_cluster);
    printf("ntfs_starting_cluster_number is 0x%llx == %lld\n", MY_NTFS.mft_mirror, MY_NTFS.mft_mirror);
    printf("serial number is 0x%llx\n", MY_NTFS.serial_number);

    return;
}

// PRE: An EXT4_SUPER_BLOCK struct
// POST: Prints the information about the ext4 filesystem
void printEXT4(EXT4_SUPER_BLOCK MY_EXT4)
{
    printf("Subtype: Ext4\n");
    printf("INODES %x\n", MY_EXT4.s_inodes_count);
    printf("BLOCKS %x\n", MY_EXT4.s_blocks_count_lo);
    printf("MAGIC %x\n", MY_EXT4.s_magic);
    printf("Last mounted: %s\n", MY_EXT4.s_last_mounted);
    printf("volume Name: %s\n", MY_EXT4.s_volume_name);
    printf("Volume ID: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x", MY_EXT4.s_uuid[i]);
    }
    printf("\n");

    return;
}