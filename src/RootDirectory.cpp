#include <errno.h>

#include "myfs-structs.h"
#include "root.h"


//create new empty filestats Array
RootDirectory::RootDirectory() {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        rootArray[i].size = -1;
    }
    DIR_STATS = {};
    strcpy(DIR_STATS.name, ".");
    DIR_STATS.mode = S_IFDIR | 0775;
    DIR_STATS.userID = geteuid();
    DIR_STATS.groupID = getegid();
    time_t currentTime = time(NULL);
    DIR_STATS.last_time = currentTime;
    DIR_STATS.change_time = currentTime;
    DIR_STATS.modi_time = currentTime;
    DIR_STATS.nlink = 2;
}

RootDirectory::~RootDirectory() {
    delete[] rootArray;
}

//return full filestats array (for writing to hard driver)
void RootDirectory::getAll(fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        *(filestats + i) = rootArray[i];
    }
}

//set filestats array (for reading from hard driver)
void RootDirectory::setAll(fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++){
        rootArray[i] = *(filestats + i);
    }
}

// deletes the filestats with the given name.
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
            fileStats stats = {};
            strcpy(stats.name, name);
            stats.userID = geteuid();
            stats.groupID = getegid();
            time_t currentTime = time(NULL);
            stats.modi_time = currentTime;
            stats.last_time = currentTime;
            stats.change_time = currentTime;
            stats.mode = S_IFREG | mode; // regular file
            stats.nlink = 1;
            stats.first_block = FAT_TERMINATOR;
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

// get the filestats of the given file, returns a number that can be used as a file descriptor
int RootDirectory::get(const char* name, fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, name) == 0) {
            *filestats = rootArray[i];
            return i;
        }
    }
    errno = ENOENT;
    return -1;
}

// set the filestats of the given file to the given values, if it exists (names are compared).
int RootDirectory::update(fileStats filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size >= 0 && strcmp(rootArray[i].name, filestats.name) == 0) {
            DIR_STATS.size -= rootArray[i].size;
            DIR_STATS.size += filestats.size;
            rootArray[i] = filestats;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}


//return filestats of the file under given number
int RootDirectory::get(int index, fileStats* filestats) {
    if (index < ROOT_ARRAY_SIZE) {
        *filestats = rootArray[index];
        return 0;
    }
    return -1;
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

int RootDirectory::getName(int index, char** name) {
    if (index <= ROOT_ARRAY_SIZE) {
        if (rootArray[index].size >= 0) {
            *name = rootArray[index].name;
            return 0;
        }
        errno = ENOENT;
        return -1;
    } else {
        errno = ENXIO;
        return -1;
    }
}

//get filestats info from new file and add it to given position
//in array
int RootDirectory::set(int num, char* filePath) {
    struct stat sb;
    stat(filePath, &sb);
    char *filename = basename(filePath);
    if (strlen(filename) > NAME_LENGTH) {
        return -1;
    }
    fileStats* status = new fileStats;
    strcpy(status->name, filename);
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->modi_time = sb.st_mtime;
    time(&(status->last_time));
    time(&(status->change_time));
    rootArray[num] = *status;

    delete status;
    return 0;
}
