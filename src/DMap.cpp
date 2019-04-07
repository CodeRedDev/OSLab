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
    for (int i = 0; i < DATA_BLOCKS + 1; i++){
        blocks[i] = true;
    }
    firstFreeBlock = 0;
}

bool DMap::isBlockFree(uint16_t blockNo){
    return blocks[blockNo];
}

uint16_t DMap::getNextFreeBlock(uint16_t startBlock) {
    for(int i = startBlock; i < DATA_BLOCKS; i++){
        if(isBlockFree(i)){
            return (uint16_t) i;
        }
    }
    return DATA_BLOCKS;
}

void DMap::freeBlock(uint16_t blockNo) {
    setBlock(blockNo, true);
    if(blockNo < firstFreeBlock){
        firstFreeBlock = blockNo;
    }
}

void DMap::setBlockUsed(uint16_t blockNo) {
    setBlock(blockNo, false);

    if(blockNo == firstFreeBlock){
        for(int i = blockNo; i < DATA_BLOCKS; i++){
            if(blocks[i]){
                firstFreeBlock = (uint16_t) i;
                break;
            }else if( !blocks[i] && i == DATA_BLOCKS){
                firstFreeBlock = (uint16_t) DATA_BLOCKS;
            }
        }
    }
}

void DMap::setBlock(uint16_t blockNo, bool free) {
    blocks[blockNo] = free;
}

int DMap::getAFreeBlock() {
    if (firstFreeBlock < DATA_BLOCKS){
        return firstFreeBlock;
    }else{
        errno = ENOSPC;
        return -1;
    }
}

bool* DMap::getDMap() {
    return blocks;
}

void DMap::setDMap(bool * dMapArray) {
    for(int i = 0; i < DATA_BLOCKS; i++){
        blocks[i] = *(dMapArray + i);
    }
    firstFreeBlock = getNextFreeBlock(0);
}



