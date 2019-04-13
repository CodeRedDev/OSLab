#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <vector>
#include "constants.h"

class Fat {
private:
    uint16_t fatList[DATA_BLOCKS];
public:
    Fat();
    ~Fat();
    int setNextBlock(uint16_t currentBlock, uint16_t nextBlock);
    void setEndOfFile(uint16_t block);
    uint16_t getNextBlock(uint16_t currentBlock);
    std::vector<uint16_t> getAssociatedBlocks(uint16_t startBlock);
    uint16_t* getFatList();
    void setFat(uint16_t* fatList);
};

#endif