//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//


#ifndef myfs_structs_h
#define myfs_structs_h

#include <stdlib.h>
#include <string>
#include "constants.h"

// TODO: Add structures of your file system here


typedef struct{

    size_t fsSize;
    size_t freeSpace;
    size_t maxSpace;

    uint16_t dmapStart;
    size_t dmapSize;

    uint16_t fatStart;
    size_t fatSize;

    uint16_t rootStart;
    size_t rootSize;

    uint16_t dataStart;
    size_t dataSize;

} SuperBlock;

typedef struct{

    char name[NAME_LENGTH];
    off_t size;

    uid_t userID;
    gid_t groupID;

    time_t lastAccess;
    time_t lastChange;

    uint16_t firstBlock;

    mode_t readWriteExecuteRighs;

    nlink_t nlink;


} FileInfo;

typedef struct {
    int rootIndex;
    bool read;
    bool write;
    uint16_t bufferBlockNumber; // block number in data section (without DATA_START)
    char buffer[BLOCK_SIZE];
} OpenFile;

#endif /* myfs_structs_h */
