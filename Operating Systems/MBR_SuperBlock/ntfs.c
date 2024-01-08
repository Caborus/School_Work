#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char *argv[])
{
    NTFS_LAYOUT MY_NTFS;
    FILE *fp;
    char *filename;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    filename = argv[1];

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }

    fread(&MY_NTFS, sizeof(NTFS_LAYOUT), 1, fp);

    printf("sector_size is 0x%x\n", MY_NTFS.sector_size);
    printf("ntfs_sectors_per_track is 0x%x\n", MY_NTFS.sectors_per_track);
    printf("ntfs_heads is 0x%x\n", MY_NTFS.number_of_heads);
    printf("ntfs_OS is 0x");
    for (int i = 0; i < 4; i++)
    {
        printf("%x", MY_NTFS.os[i]);
    }

    printf("\nntfs_total_sectors is 0x%llx == %lld\n", MY_NTFS.total_sectors, MY_NTFS.total_sectors);
    printf("ntfs_starting_cluster is 0x%llx == %lld\n", MY_NTFS.starting_cluster, MY_NTFS.starting_cluster);
    printf("ntfs_starting_cluster_number is 0x%llx == %lld\n", MY_NTFS.mft_mirror, MY_NTFS.mft_mirror);
    printf("serial number is 0x%llx\n", MY_NTFS.serial_number);

    fclose(fp);

    return 0;
}