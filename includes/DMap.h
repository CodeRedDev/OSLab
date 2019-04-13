//
// Created by Mika Auer on 2019-04-07.
//

#ifndef DMAP_H
#define DMAP_H

#include <cstdint>
#include "constants.h"

class DMap{

    uint16_t firstFreeBlock;

    //set false if block is used, true if its free
    bool blocks[DATA_BLOCKS];

    uint16_t getNextFreeBlock(uint16_t startBlock);
    bool isBlockFree(uint16_t blockNo);
    void setBlock(uint16_t blockNo, bool free);

public:
    DMap();
    ~DMap() = default;

    void freeBlock(uint16_t blockNo);
    void setBlockUsed(uint16_t blockNo);

    uint16_t getAFreeBlock();

    bool* getDMap();
    void setDMap(bool* dMapArray);

};

#endif //DMAP_H
