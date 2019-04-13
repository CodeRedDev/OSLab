#ifndef ROOTDIRECTORY_H
#define ROOTDIRECTORY_H

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>  //basename
#include <unistd.h>  //getuid /getgid
#include <ctime>  //Time()

#include "constants.h"
#include "myfs-structs.h"

#define ROOT_ARRAY_SIZE (NUM_DIR_ENTRIES + 1)
#define DIR_STATS rootArray[ROOT_ARRAY_SIZE - 1]

class RootDirectory {
private:
    fileInfo *rootArray = new fileInfo[ROOT_ARRAY_SIZE];
public:
    RootDirectory();
    ~RootDirectory();
    void getAll(fileInfo* fileInfo);
    void setAll(fileInfo* fileInfo);

    //Methods with filename:

    int get(const char* name, fileInfo* fileInfo);
    int update(fileInfo fileInfo);
    int deleteEntry(const char* name);
    int createEntry(const char* name, mode_t mode);
    int createEntry(const char* name);
    int rename (const char* oldname, const char* newname);

    //Methods with position(--> file descriptor):

    int get(int num, fileInfo* fileInfo);
    int set(int num, char* filePath);
    bool exists(int index);
    int getName(int index, char** name);

};


#endif //ROOTDIRECTORY_H
