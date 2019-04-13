//
//  myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment this to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include <errno.h>
#include <string>

#include "macros.h"

#include "myfs.h"
#include "myfs-info.h"
#include <string.h>
#include <iostream>

MyFS* MyFS::_instance = NULL;

MyFS* MyFS::Instance() {
    if(_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
    this->logFile= stderr;
}

MyFS::~MyFS() {
    
}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    
    // TODO: Implement this!

    const char* filename = path;

    if (*path == '/') {
        if (strlen(path) == 1) {
            filename = ".";
        } else {
            filename++;
        }
    }

    FileInfo fileInfo;
    int fileDesc = this->rootDir.get(filename, &fileInfo);
    if(fileDesc < 0){
        std::cerr << "No file found for path: " << *path << std::endl << "Error number:" << errno << std::endl;
        RETURN (-errno);
    }

    statbuf->st_size = fileInfo.size;

    statbuf->st_uid = fileInfo.userID;
    statbuf->st_gid = fileInfo.groupID;

    statbuf->st_mode = fileInfo.readWriteExecuteRighs;

    statbuf->st_atime = fileInfo.lastAccess;
    statbuf->st_ctime = fileInfo.lastChange;
    statbuf->st_mtime = fileInfo.lastChange;

    statbuf->st_nlink = fileInfo.nlink;

    RETURN(0);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseRmdir(const char *path) {
    LOGM();
    return 0;
}

int MyFS::fuseSymlink(const char *path, const char *link) {
    LOGM();
    return 0;
}

int MyFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    const char* name = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
            name = ".";
        } else {
            name++;
        }
    }
    LOGF("File name: %s", name);
    
    FileInfo file;
    int rootIndex = rootDir.get(name, &file);
    if (rootIndex == -1) {
        RETURN(-errno);
    }
    
    bool success = false;
    bool read = false;
    bool write = false;
    
    if (file.userID == geteuid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("User RDWR");
            if ((file.readWriteExecuteRighs & S_IRWXU) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            if ((file.readWriteExecuteRighs & S_IWUSR) != 0) {
                LOG("User WRONLY");
                write = true;
                success = true;
            }
        } else {
            LOG("User RDONLY");
            if ((file.readWriteExecuteRighs & S_IRUSR) != 0) {
                read = true;
                success = true;
            }
        }
    } else if (file.groupID == getegid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Group RDWR");
            if ((file.readWriteExecuteRighs & S_IRWXG) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Group WRONLY");
            if ((file.readWriteExecuteRighs & S_IWGRP) != 0) {
                write = true;
                success = true;
            }
        } else {
            LOG("Group RDONLY");
            if ((file.readWriteExecuteRighs & S_IRGRP) != 0) {
                read = true;
                success = true;
            }
        }
    } else {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Other RDWR");
            if ((file.readWriteExecuteRighs & S_IRWXO) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Other WRONLY");
            if ((file.readWriteExecuteRighs & S_IWOTH) != 0) {
                write = true;
                success = true;
            }
        } else {
            LOG("Other RDONLY");
            if ((file.readWriteExecuteRighs & S_IROTH) != 0) {
                read = true;
                success = true;
            }
        }
    }

    if (success) {
        for (int i = 0; i < NUM_OPEN_FILES; i++) {
            if (openFiles[i].rootIndex < 0) {
                openFiles[i].rootIndex = rootIndex;
                openFiles[i].write = write;
                openFiles[i].read = read;
                fileInfo->fh = i;
                RETURN(0);
            }
        }
        errno = EMFILE;
	    RETURN(-errno);
    } else {
        errno = EACCES;
        RETURN(-errno);
    }
    
    RETURN(0);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    if (offset < 0){
        offset = 0;
    }

    int fileDescriptor = fileInfo->fh;

    if ((fileDescriptor < 0) || (fileDescriptor >= NUM_DIR_ENTRIES)) {
        errno = EBADF;
        RETURN(-errno);
    }

    // Check if file is already being read
    if (this->openFiles[fileDescriptor].rootIndex < 0 || !(this->openFiles[fileDescriptor].read)) {
        errno = EBADF;
        RETURN(-errno);
    }

    // Get file info
    int rootIndex = this->openFiles[fileDescriptor].rootIndex;
    FileInfo file;
    if (this->rootDir.get(rootIndex, &file) == -1){
        RETURN(-errno);
    }
    if (file.size <= offset){
        RETURN(0);
    }
    if ((uint64_t)file.size < offset + size) {//TODO uint32_t
        size = file.size - offset;//TODO WHY?
    }

    // Set the lastAccess time to the current time
    file.lastAccess = time(NULL);
    this->rootDir.update(file);

    off_t blockNo = offset / BLOCK_SIZE; // block number of file (not block number in filesystem!)
    off_t blockOffset = offset % BLOCK_SIZE; // offset in the block

    //number of blocks you need to read for this operation (upper limit)
    int fileBlockCount = (size + blockOffset) / BLOCK_SIZE;
    if ((size + blockOffset) % BLOCK_SIZE != 0) {
        fileBlockCount++;
    }

    uint16_t currentBlock = file.firstBlock;
    uint16_t blocks[fileBlockCount]; //saves all block locations needed for this operation
    for ( int t = 0; t < blockNo ; t++){
        this->fat.getNextBlock(currentBlock);
    }

    for(int i = 0; i < fileBlockCount; i++){ // save blocks for reading
        blocks[i] = currentBlock;
        this->fat.getNextBlock(currentBlock);
    }

    char buffer[BLOCK_SIZE];
    size_t readSize;
    if (blockOffset + size < BLOCK_SIZE) {
        readSize = size;
    } else {
        readSize = BLOCK_SIZE - (size_t)blockOffset;
    }
    if (this->openFiles[fileDescriptor].bufferBlockNumber == blocks[0]) {
        memcpy(buf, this->openFiles[fileDescriptor].buffer + blockOffset, readSize);
    } else {
        blockDevice->read(DATA_START + blocks[0], buffer);
        memcpy(buf, buffer + blockOffset, readSize);
        if (fileBlockCount == 1) {
            memcpy(this->openFiles[fileDescriptor].buffer, buffer, BLOCK_SIZE);
            this->openFiles[fileDescriptor].bufferBlockNumber = blocks[fileBlockCount - 1];
        }
    }
    for (int j = 1; j < fileBlockCount - 1; j++) {
        //First read block size was BLOCK_SIZE - blockOffset. This value has to be added to the next read operations.
        this->blockDevice->read(DATA_START + blocks[j], buf - blockOffset + BLOCK_SIZE * j);
        LOGF("Block %d wird gelesen",blocks[j]);//new testing TODO delete
    }
    if (fileBlockCount > 1) {
        readSize = (size + blockOffset) % BLOCK_SIZE;
        if (readSize == 0) readSize = BLOCK_SIZE;
        this->blockDevice->read(DATA_START + blocks[fileBlockCount - 1], buffer);
        memcpy(buf - blockOffset + (fileBlockCount - 1) * BLOCK_SIZE, buffer, readSize);

        memcpy(openFiles[fileDescriptor].buffer, buffer, BLOCK_SIZE);
        this->openFiles[fileDescriptor].bufferBlockNumber = blocks[fileBlockCount - 1];
    }

    RETURN((int)size);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    if (fileInfo->fh < 0 || fileInfo->fh >= NUM_DIR_ENTRIES) {
        errno = EBADF;
        RETURN(-errno);
    }
    if (openFiles[fileInfo->fh].rootIndex >= 0) {
        openFiles[fileInfo->fh].rootIndex = -1;
        openFiles[fileInfo->fh].read = false;
        openFiles[fileInfo->fh].write = false;
        openFiles[fileInfo->fh].bufferBlockNumber = FAT_EOF;
        RETURN(0);
    } else {
        errno = EBADF;
        RETURN(-errno);
    }
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return 0;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    if (strcmp("/", path) == 0) {
        for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
            if (this->rootDir.exists(i)) {
                struct stat s = {};
                char* name;
                this->rootDir.getName(i, &name);
                fuseGetattr(name, &s);
                filler(buf, name, &s, 0);
            }
        }
        filler(buf, "..", NULL, 0);
        
        RETURN(0);
    } else {
        errno = ENOTDIR;
        RETURN(-errno);
    }
    
    // <<< My new code
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

void MyFS::fuseDestroy() {
    LOGM();
}

void* MyFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        //    this->logFile= ((MyFsInfo *) fuse_get_context()->private_data)->logFile;
        
        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);
        
        LOG("Starting logging...\n");
        LOGM();
        
        // you can get the containfer file name here:
        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);
        
        this->initializeFilesystem(((MyFsInfo *) fuse_get_context()->private_data)->contFile);
    }
    
    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
#else
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
#endif
    LOGM();
    RETURN(0);
}
    
#ifdef __APPLE__
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x) {
#else
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
#endif
    LOGM();
    RETURN(0);
}
        
// TODO: Add your own additional methods here!

/**
 * Initializes our Filesystem with the content of the given containerFile
 *
 * @param char* containerFile
 * @return int
 */
int MyFS::initializeFilesystem(char *containerFile) {
    if (blockDevice->open(containerFile) == 0) {
        bool *dMapBlocks = new bool[DATA_BLOCKS];
        uint16_t *fatList = new uint16_t[DATA_BLOCKS];
        FileInfo *rootArray = new FileInfo[ROOT_ARRAY_SIZE];

        int ret = 0;
        ret = this->blockDeviceHelper.readDevice(SUPERBLOCK_START, &this->superBlock, sizeof(this->superBlock));
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading superblock)");
        }
        ret = this->blockDeviceHelper.readDevice(DMAP_START, dMapBlocks, sizeof(*dMapBlocks) * (DATA_BLOCKS + 1) / 8);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading dmap)");
        }
        ret = this->blockDeviceHelper.readDevice(FAT_START, fatList, sizeof(*fatList) * DATA_BLOCKS);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading fat)");
        }
        ret = this->blockDeviceHelper.readDevice(ROOTDIR_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading root)");
        }

        this->dMap.setDMap(dMapBlocks);
        this->fat.setFat(fatList);
        this->rootDir.setAll(rootArray);

        delete[] dMapBlocks;
        delete[] fatList;
        delete[] rootArray;

        for (int i = 0; i < NUM_OPEN_FILES; i++) {
            this->openFiles[i].rootIndex = -1;
            this->openFiles[i].read = false;
            this->openFiles[i].write = false;
            this->openFiles[i].bufferBlockNumber = -1;
        }

        LOG("Successfully initialized the filesystem");

        RETURN(0);
    }
    LOG("Error at blockdevice.open()");
    RETURN(-1);
}