//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "constants.h"
#include "BlockDeviceHelper.h"
#include "myfs-structs.h"
#include "Fat.h"
#include "DMap.h"
#include "RootDirectory.h"

int main(int argc, char *argv[]) {
    // TODO: Implement file system generation & copying of files here
    BlockDevice *device = new BlockDevice();
    BlockDeviceHelper deviceHelper = BlockDeviceHelper(device);
    
    
    SuperBlock superBlock = {};
    Fat fat = Fat();
    DMap dMap = DMap();
    RootDirectory rootDir = RootDirectory();
    
    if (argc > 1) {
        int ret = 0;
        
        // Remove container
        remove(argv[1]);
        device->create(argv[1]);
        
        if (argc > 2) {
            // Check file size
            int availableSpace = DATA_BLOCKS * BLOCK_SIZE;
            struct stat sizeBuffer = {};
            for (int i = 2; i < argc; i++) {
                stat(argv[i], &sizeBuffer);
                availableSpace -= (int)sizeBuffer.st_size; // Why cast?
            }
            
            if (availableSpace < 0) {
                std::cout << "The files to copy are too large for the file system. Aborting..." << std::endl;
                return EXIT_FAILURE;
            }
            
            struct stat fileStatBuffer = {};
            
            // Copy files
            for (int i = 2; i < argc; i++) {
                char* filename = basename(argv[i]);
                
                int fileDesc = open(filename, O_RDONLY);
                
                if (fileDesc < 0) {
                    std::cout << "Failed at opening file on errorno: " << errno << std::endl;
                    return errno;
                } else {
                    stat(argv[i], &fileStatBuffer);
                    ret = rootDir.createEntry(filename, fileStatBuffer.st_mode);
                    if (ret < 0) {
                        std::cerr << "RootDirectory: Failed to create entry caused by: " << errno << std::endl;
                        return errno;
                    }
                    FileInfo* fileInfo;
                    // Get file stats
                    fileInfo = rootDir.get(filename);
                    ret = -1;
                    if(fileInfo != nullptr) {
                        ret = rootDir.getPosition(fileInfo);
                    }
                    fileInfo->lastChange = fileStatBuffer.st_ctime;
                    ret = rootDir.update(*fileInfo);
                    if (ret < 0) {
                        std::cerr << "RootDirectory: Failed to update info caused by: " << errno << std::endl;
                        return errno;
                    }

                    uint16_t nextBlock = dMap.getAFreeBlock();
                    uint16_t currentBlock;
                    if (nextBlock < 0) {
                        std::cerr << "DMap: Failed to get free block caused by: " << errno << std::endl;
                        return errno;
                    }
                    fileInfo->firstBlock = nextBlock;
                    
                    // Write data
                    char buffer[BLOCK_SIZE];
                    ssize_t fileStream = read(fileDesc, buffer, BLOCK_SIZE);
                    while (fileStream > 0) {
                        ret = device->write(DATA_START + nextBlock, buffer);
                        if (ret < 0) {
                            std::cerr << "Failed to write data to device: " << errno << std::endl;
                            return errno;
                        }
                        dMap.setBlockUsed(nextBlock);
                        fileInfo->size += fileStream;
                        
                        currentBlock = nextBlock;
                        nextBlock = dMap.getAFreeBlock();
                        if (nextBlock < 0) {
                            std::cerr << "DMap: Failed to get free block caused by: " << errno << std::endl;
                            return errno;
                        }
                        
                        ret = fat.setNextBlock(currentBlock, nextBlock);
                        if (ret < 0) {
                            std::cerr << "Failed to add to FAT: " << errno << std::endl;
                            return errno;
                        }
                        
                        fileStream = read(fileDesc, buffer, BLOCK_SIZE);
                    }
                    
                    fat.setEndOfFile(nextBlock);
                    ret = rootDir.update(*fileInfo);
                    if (ret < 0) {
                        std::cerr << "RootDirectory: Failed to update info caused by: " << errno << std::endl;
                        return errno;
                    }
                    superBlock.freeSpace -= fileInfo->size;
                    close(fileDesc);
                }
            }
        }
        
        FileInfo* rootArray = rootDir.getAll();
        
        ret = deviceHelper.writeDevice(SUPERBLOCK_START, &superBlock, sizeof(superBlock));
        if (ret < 0) {
            std::cerr << "Failed to write SuperBlock (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(FAT_START, fat.getFatList(), sizeof(*(fat.getFatList())) * DATA_BLOCKS);
        if (ret < 0) {
            std::cerr << "Failed to write FAT (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(DMAP_START, dMap.getDMap(), sizeof(*(dMap.getDMap())) * DATA_BLOCKS);
        if (ret < 0) {
            std::cerr << "Failed to write DMap (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(ROOTDIR_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
        if (ret < 0) {
            std::cerr << "Failed to write RootDirectory (" << ret << ") caused by:" << errno << std::endl;
        }
    } else {
        std::cerr << "Invalid arguments: Missing container file!" << std::endl;
    }
    
    return EXIT_SUCCESS;
}
