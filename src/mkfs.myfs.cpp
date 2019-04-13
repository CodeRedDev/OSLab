//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "constants.h"
#include "BlockDeviceHelper.h"
#include "myfs-structs.h"
#include "DMap.h"
#include "Fat.h"

int main(int argc, char *argv[]) {
    // TODO: Implement file system generation & copying of files here
    BlockDevice *device = new BlockDevice();
    BlockDeviceHelper deviceHelper = BlockDeviceHelper(device);
    
    SuperBlock superBlock = {};
    DMap dMap = DMap();
    Fat fat = Fat();
    
    if (argc > 1) {
        int ret = 0;
        
        ret = deviceHelper.writeDevice(SUPERBLOCK_START, &superBlock, sizeof(superBlock));
        if (ret < 0) {
            std::cout << "Failed to write SuperBlock (" << ret << ") caused by:" << errno << std::endl;
        }
        
        ret = deviceHelper.writeDevice(DMAP_START, dMap.getDMap(), sizeof(*(dMap.getDMap())));
        if (ret < 0) {
            std::cout << "Failed to write DMap (" << ret << ") caused by:" << errno << std::endl;
        }
        
        // TODO: Write Fat and Root
    }

    //Erstellt neues device mit path  der im Terminal übergeben wird
    device->create(argv[1]);
    return 0;
}
