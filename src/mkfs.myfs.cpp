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
    
    fileStats* rootArray = new fileStats[ROOT_ARRAY_SIZE];
    rootDir.getAll(rootArray);
    
    if (argc > 1) {
        int ret = 0;
        
        device->create(argv[1])
        
        /*
        ret = deviceHelper.writeDevice(SUPERBLOCK_START, &superBlock, sizeof(superBlock));
        if (ret < 0) {
            std::cerr << "Failed to write SuperBlock (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(FAT_START, fat.getFat(), sizeof(*(fat.getFatList())) * DATA_BLOCKS);
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
        */
        
        if (argc > 2) {
            // TODO: file length check?
            struct stat buffer = {};
            struct stat bufferTime = {};
            
            // Copy files
            for (int i = 2; i < argc; i++) {
                char* filename = basename(argv[i]);
                
                int fileDesc = open(filename, O_RDONLY);
                
                if (fileDesc < 0) {
                    return errno;
                } else {
                    stat(argv[i], &bufferTime);
                    ret = rootDir.createEntry(filename, bufferTime.st_mode);
                    if (ret < 0) {
                        std::cerr << "RootDirectory: Failed to create entry (" << ret << ") caused by:" << errno << std::endl;
                        return errno;
                    }
                     stats;
                    // Get file stats
                    ret = rootDir.get(filename, &stats);
                    stats.lastChange = bufferTime.st_ctime;
                }
            }
        }
        
        
        // TODO: Get modified rootArray
        
        ret = deviceHelper.writeDevice(SUPERBLOCK_START, &superBlock, sizeof(superBlock));
        if (ret < 0) {
            std::cerr << "Failed to write SuperBlock (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(FAT_START, fat.getFat(), sizeof(*(fat.getFatList())) * DATA_BLOCKS);
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
    }
    
    return 0;
}
