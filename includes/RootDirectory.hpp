//
//  RootDirectory.hpp
//  mkfs.myfs
//
//  Created by Mika Auer on 21.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#ifndef RootDirectory_hpp
#define RootDirectory_hpp

#include <stdio.h>
#include "RootData.h"

class RootDirectory{
    RootData directory[64];
};

#endif /* RootDirectory_hpp */
