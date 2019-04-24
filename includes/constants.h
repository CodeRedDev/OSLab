#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * Put all constant values here that define the file systems behaviour.
 */

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64
#define DATA_BLOCKS 62500
#define FAT_EOF -1

/**
 * How to get component block size/count. Example for FAT:
 * 
 * 32MB = 32.000.000 byte (wanted memory)
 * 
 * 32.000.000 / 512 = 62500 (blocks to write)
 * 
 * For every block you need a "point-to-next-free" entry in the FAT array.
 * For u_int16_t:
 * 
 * 62500 * 16 bit = 1.000.000 bit
 * tobyte
 * 1.000.000 bit / 8 = 125.000 byte
 * 
 * Blocks needed for FAT
 * 125.000 byte / 512 byte = 244,141... => 255 blocks 
 */
 
#define SUPERBLOCK_SIZE 1
#define FAT_SIZE 255
#define DMAP_SIZE 123 // TODO: When saving only the map 123 blocks will be enough
#define ROOTDIR_SIZE 39 // FileInfo struct has size of 285 byte (rootdir holds array of 65 FileInfo) => 18525byte
// TODO: Should take 37 blocks but works with 39... WHYYYYYYYYY?

#define SUPERBLOCK_START 0
#define FAT_START (SUPERBLOCK_START + SUPERBLOCK_SIZE)
#define DMAP_START (FAT_START + FAT_SIZE)
#define ROOTDIR_START (DMAP_START + DMAP_SIZE)
#define DATA_START (ROOTDIR_START + ROOTDIR_SIZE)

#endif