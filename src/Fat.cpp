#include "Fat.h"

Fat::Fat() {
    for (int i = 0; i < DATA_BLOCKS; i++) {
        fatList[i] = FAT_EOF;
    }
}

Fat::~Fat() {
    // Nothing to clear
}

int Fat::setNextBlock(uint16_t currentBlock, uint16_t nextBlock) {
    if (currentBlock == nextBlock) {
        return -1;
    }
    fatList[currentBlock] = nextBlock;
    return 0;
}

uint16_t Fat::getNextBlock(uint16_t currentBlock) {
    return fatList[currentBlock];
}

std::vector<uint16_t> Fat::getAssociatedBlocks(uint16_t startBlock) {
    std::vector<uint16_t> blocks{startBlock};
    uint16_t currentBlock = startBlock;
    
    while (getNextBlock(currentBlock) != FAT_EOF) {
        blocks.push_back(getNextBlock(currentBlock));
        currentBlock = getNextBlock(currentBlock);
    }
    return blocks;
}