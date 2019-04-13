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

    if(*path == '/'){
        if(std::strlen(path) == 1){
            filename = ".";
        }else{
            filename++;
        }
    }

    FileInfo fileInfo;
    int fileDesc = this->rootDir.get(filename, &fileInfo);
    if(fileDesc < 0){
        std::cerr << "No file found for path: " << *path << std::endl << "Error number:" << errno << std::endl;
        return errno;
    }

    statbuf->st_size = fileInfo.size;

    statbuf->st_uid = fileInfo.userID;
    statbuf->st_gid = fileInfo.groupID;

    statbuf->st_mode = fileInfo.readWriteExecuteRighs;

    statbuf->st_atime = fileInfo.lastAccess;
    statbuf->st_ctime = fileInfo.lastChange;
    statbuf->st_mtime = fileInfo.lastChange;

    statbuf->st_nlink = fileInfo.nlink;

    return 0;
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
    
    RETURN(0);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!

    RETURN(0);
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
        openFiles[fileInfo->fh].rootInedx = -1;
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
        FileInfo *rootArray = new FileInfo[ROOT_ARRAY_SIZE];
        uint16_t *fatList = new uint16_t[DATA_BLOCKS];

        int ret = 0;
        ret = this->blockDeviceHelper.readDevice(SUPERBLOCK_START, &this->superBlock, sizeof(this->superBlock));
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading superblock)");
            LOGI(ret);
            LOG("Errno:");
            LOGI(errno);
        }
        ret = this->blockDeviceHelper.readDevice(DMAP_START, dMapBlocks, sizeof(*dMapBlocks) * (DATA_BLOCKS + 1) / 8);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading dmap)");
            LOGI(ret);
            LOG("Errno:");
            LOGI(errno);
        }
        ret = this->blockDeviceHelper.readDevice(FAT_START, fatList, sizeof(*fatArray) * DATA_BLOCKS);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading fat)");
            LOGI(ret);
            LOG("Errno:");
            LOGI(errno);
        }
        ret = this->blockDeviceHelper.readDevice(ROOT_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
        if (ret < 0) {
            LOG("Error at blockdevice.read() (Reading root)");
            LOGI(ret);
            LOG("Errno:");
            LOGI(errno);
        }

        this->dMap.setDMap(dMapArray);
        this->fat.setFat(fatArray);
        this->rootDir.setAll(rootArray);

        delete[] dMapArray;
        delete[] fatArray;
        delete[] rootArray;

        for (int i = 0; i < NUM_OPEN_FILES; i++) {
            this->openFiles[i].rootIndex = -1;
            this->openFiles[i].read = false;
            this->openFiles[i].write = false;
            this->openFiles[i].bufferBlockNumber = FAT_TERMINATOR;
        }

        LOG("Successfully initialized the filesystem");

        RETURN(0);
    }
    LOG("Error at blockdevice.open()");
    RETURN(-1);
}