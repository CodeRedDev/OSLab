#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class Logger {
    public:
        static void log(int ret, char *errorPlace, int errno);

        static bool debug;
};

#endif //LOGGER_H
