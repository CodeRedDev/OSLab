#include <errno.h>
#include <iostream>

#include "myfs-structs.h"
#include "RootDirectory.h"


//create new empty fileInfo Array
RootDirectory::RootDirectory() {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        rootArray[i].size = -1;
    }
    DIR_STATS = {};
    strcpy(DIR_STATS.name, ".");
    DIR_STATS.readWriteExecuteRighs = S_IFDIR | 0775;
    DIR_STATS.userID = geteuid();
    DIR_STATS.groupID = getegid();
    time_t currentTime = time(NULL);
    DIR_STATS.lastAccess = currentTime;
    DIR_STATS.lastChange = currentTime;
    DIR_STATS.nlink = 2;
}

RootDirectory::~RootDirectory() {
    delete[] rootArray;
}

//return full fileInfo array (for writing to hard driver)
FileInfo* RootDirectory::getAll() {
    FileInfo* fileInfos = new FileInfo[ROOT_ARRAY_SIZE];
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        fileInfos[i] = rootArray[i];
    }
    return fileInfos;
}

//set fileInfo array (for reading from hard driver)
void RootDirectory::setAll(FileInfo* fileInfo) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++){
        rootArray[i] = *(fileInfo + i);
    }
}

// deletes the fileInfo with the given name.
int RootDirectory::deleteEntry(const char *name) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, name) == 0) {
            DIR_STATS.size -= rootArray[i].size;
            rootArray[i] = {};
            rootArray[i].size = -1;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}

// creates a new root entry for the file with the given name.
int RootDirectory::createEntry(const char *name, mode_t mode) {
    if (strlen(name) > NAME_LENGTH) {
        errno = ENAMETOOLONG;
        return -1;
    }
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, name) == 0) {
            errno = EEXIST;
            return -1;
        }
    }
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size < 0) {
            FileInfo stats = {};
            strcpy(stats.name, name);
            stats.userID = geteuid();
            stats.groupID = getegid();
            time_t currentTime = time(NULL);
            stats.lastAccess = currentTime;
            stats.lastChange = currentTime;
            stats.readWriteExecuteRighs = S_IFREG | mode; // regular file
            stats.nlink = 1;
            stats.firstBlock = FAT_EOF;
            rootArray[i] = stats;
            return 0;
        }
    }
    errno = ENFILE;
    return -1;
}

// create file with default mode
int RootDirectory::createEntry(const char *name) {
    return createEntry(name, 0666); // default mode: read/write
}

int RootDirectory::rename(const char *oldname, const char *newname) {
    if (strlen(newname) > NAME_LENGTH) {
        errno = ENAMETOOLONG;
        return -1;
    }
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, oldname) == 0) {
            strcpy(rootArray[i].name, newname);
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}

// get the fileInfo of the given file, returns a number that can be used as a file descriptor
FileInfo* RootDirectory::get(const char* name) {
    FileInfo* fileInfo;
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, name) == 0) {
            fileInfo = &rootArray[i];
            return fileInfo;
        }
    }
    errno = ENOENT;
    return nullptr;
}

int RootDirectory::getPos(FileInfo* fileInfo){
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if(strcmp(rootArray[i].name, fileInfo->name) == 0){
            return i;
        }
    }
    return -1;
}

// set the fileInfo of the given file to the given values, if it exists (names are compared).
int RootDirectory::update(FileInfo fileInfo) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, fileInfo.name) == 0) {
            DIR_STATS.size -= rootArray[i].size;
            DIR_STATS.size += fileInfo.size;
            rootArray[i] = fileInfo;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}


//return fileInfo of the file under given number
FileInfo* RootDirectory::get(int index) {
    FileInfo* fileInfo;
    if (index < ROOT_ARRAY_SIZE) {
        fileInfo = &rootArray[index];
        return fileInfo;
    }
    return nullptr;
}

///returns true if index <= Root_Array_Size
bool RootDirectory::exists(int index) {
    if (index <= ROOT_ARRAY_SIZE) {
        if (rootArray[index].size >= 0) {
            return true;
        }
    }
    return false;
}

char* RootDirectory::getName(int index) {
    if (index <= ROOT_ARRAY_SIZE) {
        if (rootArray[index].size >= 0) {
            return rootArray[index].name;
        }
        errno = ENOENT;

        return nullptr;
    } else {
        errno = ENXIO;
        return nullptr;
    }
}

//get fileInfo info from new file and add it to given position
//in array
int RootDirectory::set(int num, char* filePath) {
    struct stat sb;
    stat(filePath, &sb);
    char *filename = basename(filePath);
    if (strlen(filename) > NAME_LENGTH) {
        return -1;
    }
    FileInfo* status = new FileInfo;
    strcpy(status->name, filename);
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->lastChange = sb.st_mtime;
    time(&(status->lastAccess));
    time(&(status->lastChange));
    rootArray[num] = *status;

    delete status;
    return 0;
}
