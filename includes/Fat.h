#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <vector>
#include "constants.h"

class Fat {
private:
    int fatList[DATA_BLOCKS];
public:
    Fat();
    ~Fat();
    int setNextBlock(int currentBlock, int nextBlock);
    void setEndOfFile(uint16_t block);
    int getNextBlock(uint16_t currentBlock);
    std::vector<uint16_t> getAssociatedBlocks(uint16_t startBlock);
    int* getFatList();
    int get(uint16_t index);
    void setFat(uint16_t* fatList);
};

#endif