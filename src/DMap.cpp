//
//  DMap.cpp
//  mkfs.myfs
//
// Created by Mika Auer on 2019-04-07.
//

#include "DMap.h"
#include <errno.h>
#include <iostream>

DMap::DMap() {
    for (int i = 0; i < DATA_BLOCKS + 1; i++) {
        this->blocks[i] = true;
    }
    this->firstFreeBlock = 0;
}

bool DMap::isBlockFree(uint16_t blockNo) {
    return this->blocks[blockNo];
}

uint16_t DMap::getNextFreeBlock(uint16_t startBlock) {
    for (int i = startBlock; i < DATA_BLOCKS; i++) {
        if (this->isBlockFree(i)) {
            return (uint16_t) i;
        }
    }
    return DATA_BLOCKS;
}

void DMap::freeBlock(uint16_t blockNo) {
    this->setBlock(blockNo, true);
    if (blockNo < this->firstFreeBlock) {
        this->firstFreeBlock = blockNo;
    }
}

void DMap::setBlockUsed(uint16_t blockNo) {
    this->setBlock(blockNo, false);

    if (blockNo == this->firstFreeBlock) {
        for (int i = blockNo; i < DATA_BLOCKS; i++) {
            if (this->blocks[i]) {
                this->firstFreeBlock = (uint16_t) i;
                break;
            } else if (!this->blocks[i] && i == DATA_BLOCKS) {
                this->firstFreeBlock = (uint16_t) DATA_BLOCKS;
            }
        }
    }
}

void DMap::setBlock(uint16_t blockNo, bool free) {
    this->blocks[blockNo] = free;
}

int DMap::getAFreeBlock() {
    if (this->firstFreeBlock < DATA_BLOCKS) {
        return this->firstFreeBlock;
    } else {
        errno = ENOSPC;
        return -1;
    }
}

bool *DMap::getDMap() {
    return blocks;
}

void DMap::setDMap(bool *dMapArray) {
    for (int i = 0; i < DATA_BLOCKS; i++) {
        this->blocks[i] = *(dMapArray + i);
    }
    this->firstFreeBlock = this->getNextFreeBlock(0);
}
