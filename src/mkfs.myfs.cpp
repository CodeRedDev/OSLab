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

#define numberOfDataBlocks 62500





int main(int argc, char *argv[]) {
    // TODO: Implement file system generation & copying of files here
    BlockDevice *device = new BlockDevice();

    //Erstellt neues device mit path  der im Terminal übergeben wird
    device->create(argv[1]);
    return 0;
}
