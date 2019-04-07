//
//  rootData.hpp
//  mkfs.myfs
//
//  Created by Mika Auer on 21.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#ifndef RootData_hpp
#define RootData_hpp

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

enum Mode{
    READ = 4,
    WRITE = 2,
    EXECUTE = 1
};

struct RootData{
    char* name;
    u_int32_t size;
    u_int32_t group;
    u_int32_t user;
    u_int32_t mode;
    time_t atime;
    time_t mtime;
    time_t ctime;
    u_int32_t firstBlock;
};

#endif /* rootData_hpp */
