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
    FileInfo rootArray[ROOT_ARRAY_SIZE];
public:
    RootDirectory();
    ~RootDirectory();
    FileInfo* getAll();
    void setAll(FileInfo* fileInfo);

    //Methods with filename:

    FileInfo* get(const char* name);
    int getPosition(FileInfo* fileInfo);
    int update(FileInfo fileInfo);
    int deleteEntry(const char* name);
    int createEntry(const char* name, mode_t mode);
    int createEntry(const char* name);
    int rename (const char* oldname, const char* newname);

    //Methods with position(--> file descriptor):

    FileInfo* get(int index);
    int set(int index, char* filePath);
    bool exists(int index);
    char* getName(int index);

};


#endif //ROOTDIRECTORY_H
