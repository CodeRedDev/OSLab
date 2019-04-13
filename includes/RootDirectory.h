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
    fileStats *rootArray = new fileStats[ROOT_ARRAY_SIZE];
public:
    RootDirectory();
    ~RootDirectory();
    void getAll(fileStats* filestats);
    void setAll(fileStats* filestats);

    //Methods with filename:

    int get(const char* name, fileStats* filestats);
    int update(fileStats filestats);
    int deleteEntry(const char* name);
    int createEntry(const char* name, mode_t mode);
    int createEntry(const char* name);
    int rename (const char* oldname, const char* newname);

    //Methods with position(--> file descriptor):

    int get(int num, fileStats* filestats);
    int set(int num, char* filePath);
    bool exists(int index);
    int getName(int index, char** name);

};


#endif //ROOTDIRECTORY_H
