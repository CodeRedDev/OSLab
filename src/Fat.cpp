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
    this->fatList[currentBlock] = nextBlock;
    return 0;
}

void Fat::setEndOfFile(uint16_t block) {
    fatList[block] = FAT_EOF;
}

uint16_t Fat::getNextBlock(uint16_t currentBlock) {
    return this->fatList[currentBlock];
}

std::vector<uint16_t> Fat::getAssociatedBlocks(uint16_t startBlock) {
    std::vector<uint16_t> blocks{startBlock};
    uint16_t currentBlock = startBlock;

    while (this->getNextBlock(currentBlock) != FAT_EOF) {
        blocks.push_back(getNextBlock(currentBlock));
        currentBlock = this->getNextBlock(currentBlock);
    }
    return blocks;
}

uint16_t* Fat::getFatList() {
    return this->fatList;
}

uint16_t Fat::get(uint16_t index){
    if (index >= 0 && index < DATA_BLOCKS){
        return this->fatList[index];
    }
    return -1;
}

/**
 * Sets the internal class instance fatList to the same as the parameter fatList
 *
 * @param fatList
 * @return void
 */
void Fat::setFat(uint16_t* fatList)
{
    for (int i = 0; i < DATA_BLOCKS; i++){
        this->fatList[i] = *(fatList + i);
    }
}