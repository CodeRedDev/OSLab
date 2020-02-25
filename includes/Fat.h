#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <vector>
#include "constants.h"

class Fat {
private:
    int fatList[DATA_BLOCKS]{};
public:
    Fat();
    ~Fat();
    int setNextBlock(int currentBlock, int nextBlock);
    void setEndOfFile(int block);
    int getNextBlock(int currentBlock);
    std::vector<uint16_t> getAssociatedBlocks(uint16_t startBlock);
    int* getFatList();
    int get(int index);
    void setFat(const int* list);
};

#endif