#ifndef BLOCKDEVICEHELPER_H
#define BLOCKDEVICEHELPER_H

#include <stdint.h>
#include <stdlib.h>
#include <type_traits>
#include <string.h>

#include "blockdevice.h"
#include "constants.h"

class BlockDeviceHelper {
private:
    BlockDevice* blockDevice;
public:
    
    BlockDeviceHelper(BlockDevice* blockdevice) {
        this->blockDevice = blockDevice;
    }
    
    template<class T>
    int writeDevice(size_t block, const T *data, size_t size) {
        const char *rawData = reinterpret_cast<const char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        int ret = 0;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            ret = blockDevice->write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
            if (ret < 0) return ret;
        }
        memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), size % BLOCK_SIZE);
        ret = blockDevice->write(currentBlock, buffer);
        if (ret < 0) return ret;
        return 0;
    }
    
    template<class T>
    int readDevice(size_t block, T* data, size_t size) {
        char *rawData = reinterpret_cast<char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        int ret = 0;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            ret = blockDevice->read(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
            if (ret < 0) return ret;
        }
        ret = blockDevice->read(currentBlock, buffer);
        if (ret < 0) return ret;
        memcpy(rawData + ((currentBlock - block) * BLOCK_SIZE), buffer, size % BLOCK_SIZE);
        return 0;
    }
}

#endif