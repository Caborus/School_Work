/*
 * Alexander Cabos
 * CS 474: Operating Systems
 * Program 6: Programming Disks and Filesystems
 * Purpose: header file for NTFS filesystem layout
 */

#include <linux/types.h>

#ifndef _NTFS_H_
#define _NTFS_H_

/*
 * Structure of the super block
 */
#pragma pack(1)
typedef struct
{
    /*00*/
    __u8 dontcare[11];        // 0x00 - 0x0A
    __le16 sector_size;       // 0x0B - 0x0C the NTFS sector_size
    __u8 sectors_per_cluster; // 0x0D
    __u8 reservered0e;        // 0x0E
    __u8 reservered0f;        // 0x0F

    /*10*/
    __u8 dontcare10[8];       // 0x10 - 0x17
    __le16 sectors_per_track; // 0x18 - 0x19 sectors per track
    __le16 number_of_heads;   // 0x1A - 0x1B number of heads
    __u8 dontcare1c[4];       // 0x1C - 0x1F

    /*20*/
    __u8 dontcare20[4];   // 0x20 - 0x23
    __u8 os[4];           // 0x24 - 0x27 NTFS OS, first byte is drive number
    __le64 total_sectors; // 0x28 - 0x2F total sectors in volume

    /*30*/
    __le64 starting_cluster; // 0x30 - 0x37 starting cluster of MFT
    __le64 mft_mirror;       // 0x38 - 0x3F starting cluster of MFT mirror

    /*40*/
    __u8 dontcare40[8];   // 0x40 - 0x47
    __le64 serial_number; // 0x48 - 0x4F serial number of volume

    /*50*/
    __u8 dontcare50[4]; // 0x50 - 0x53

} NTFS_LAYOUT;

#pragma pack()

#endif