#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class Logger {
public:
    static bool debug = false;
    static void log(char* message, int ret);
};


#endif //LOGGER_H
