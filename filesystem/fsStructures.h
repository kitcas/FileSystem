/*
 Team: Last Minute
 Date Created: 3/19/2020
 */

#ifndef fsStructures_h
#define fsStructures_h
#include <stdio.h>
#endif /* fsStructures_h */

// Permissions masking values
#define OTHER_EXECUTE 0x0001
#define OTHER_WRITE 0x0002
#define OTHER_READ 0x0004
#define GROUP_EXECUTE 0x0008
#define GROUP_WRITE 0x0010
#define GROUP_READ 0x0012
#define USER_EXECUTE 0x0014
#define USER_WRITE 0x0016
#define USER_READ 0x0018

// Sizes for data types
#define FILE_NAME_LENGTH 32
#define FILE_EXTENSION_LENGTH 10
#define VOLUME_NAME_LENGHT 30

/*
 Description: File System Permissions Explained
 Source: https://en.wikipedia.org/wiki/File_system_permissions
 The read permission grants the ability to read a file. When set for a directory, this permission grants the ability to read the names of files in the directory, but not to find out any further information about them such as contents, file type, size, ownership, permissions.
 The write permission grants the ability to modify a file. When set for a directory, this permission grants the ability to modify entries in the directory, which includes creating files, deleting files, and renaming files. Note that this requires that execute is also set; without it, the write permission is meaningless for directories.
 The execute permission grants the ability to execute a file. This permission must be set for executable programs, in order to allow the operating system to run them. When set for a directory, the execute permission is interpreted as the search permission: it grants the ability to access file contents and meta-information if its name is known, but not list files inside the directory, unless read is set also.
 
 ** A bit of 0 means access denied, a bit of 1 means access granted **
 1st bit:   Other execute
 2nd bit:   Other write
 3rd bit:   Other read
 4th bit:   Group execute
 5th bit:   Group write
 6th bit:   Group read
 7th bit:   User execute
 8th bit:   User write
 9th bit:   User read
 Bits 9-16: Unused, saved for later functionalities
*/

/*
 Total bytes per directory entry: 576 bits - 72 bytes
 N directory entries per LBA: Where N = 512/64 = 8
 */
struct directoryEntry {
    // Block number in the LBA where file index is stored. This file index contains a list of all the blocks that create that file
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t fileIndexLocation;
    
    // Char array representing the name of the file
    // 256 bits
    char name[FILE_NAME_LENGTH];
    
    // Char array represeting the file type (ie: pdf, txt, xcodeproj, ...etc)
    // 80 bits
    char fileExtension[FILE_EXTENSION_LENGTH];
    
    // A 16 bit bitmap representing the file permissions
    // A bit map of 111100100 is saved as a 484
    // 16 bits [0, 65,535]
    uint16_t permissions;
    
    // Number of second since January 1st, 1970: Unix time (also known as Epoch time)
    // 32 bits [0, 4,294,967,295]
    uint32_t dateCreated;
    
    // Number of bytes a file uses
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t fileSize;
};


/*
Volume control block should be LESS than 512 bytes, in order to fit into the 0th LBA block
*/
struct volumeControlBlock {
    // Total number of bytes a HDD has
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t volumeSize;
    
    // Char array representing the name of the volume
    // Assigned by the user. If not assigned by user, generated by file system
    // 240 bits
    char volumeName[VOLUME_NAME_LENGHT];
    
    // Random number representing the HDD
    // Assigned randomly by file system - really large random value. Small chance for collision
    // 32 bits [0, 4,294,967,295]
    uint32_t volumeID;
    
    // Size of an LBA block in bytes
    // Default will be 512 bytes, should not be changed
    // 64 bits  [0, 18,446,744,073,709,551,615]
    uint64_t blockSize;
    
    // Total number of blocks in the partition
    uint64_t numBlocks;
};


/*
 Description: Keeps track of the free blocks of the LBA
*/
struct freeSpaceInformation {
    // Number of bytes a partition has free to be used
    // As files are added or removed, this number changes to represent those changes
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t freeSpace;
    
    // Lowest LBA block accessible for this partition
    // [0, 255]
    uint8_t lowestBlockAccessible;
    
    // Highest LBA block accessible for this partition
    // 32 bits [0, 18,446,744,073,709,551,615]
    uint64_t highestBlockAccessible;
    
    // Bitmap of all used spaces in the partition, where each bit represents a single block in the LBA
    // 0 indicates an used block, 1 indicates a free block
    int freeBlockBitArray[];
};


/*
 Description: File Index Explained
 Source: https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/12_FileSystemImplementation.html
 Indexed Allocation combines all of the indexes for accessing each file into a common block (for that file)
 Linked Scheme - An index block is one disk block, which can be read and written in a single disk operation
*/
struct fileIndexInformation {
    // If the fileIndexLocation[] array gets larger than 63 elements, a new LBA block is needed to store the rest of the elements
    // By default, this will be NULL and should stay NULL until the entire LBA block this index is in gets full
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t nextFileIndexLocation;
    
    // An array of block numbers in the LBA files are stored. Max size is 63 elements before LBA block is full
    // Each element can be 64 bits [0, 18,446,744,073,709,551,615] for each element in the array
    uint64_t fileIndexLocation[];
};
