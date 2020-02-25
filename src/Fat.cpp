#include "Fat.h"

Fat::Fat() {
    for (int & i : fatList) {
        i = FREE_FAT_BLOCK;
    }
}

Fat::~Fat() {
    // Nothing to clear
}

int Fat::setNextBlock(int currentBlock, int nextBlock) {
    if (currentBlock == nextBlock) {
        return -1;
    }
    this->fatList[currentBlock] = nextBlock;
    return 0;
}

void Fat::setEndOfFile(int block) {
    fatList[block] = FAT_EOF;
}

int Fat::getNextBlock(int currentBlock) {
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

int* Fat::getFatList() {
    return this->fatList;
}

int Fat::get(int index){
    if (index >= 0 && index < DATA_BLOCKS){
        return this->fatList[index];
    }
    return -1;
}

/**
 * Sets the internal class instance fatList to the same as the parameter fatList
 *
 * @param list
 * @return void
 */
void Fat::setFat(const int* list)
{
    for (int i = 0; i < DATA_BLOCKS; i++){
        this->fatList[i] = *(list + i);
    }
}