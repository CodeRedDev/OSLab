#include "../includes/Logger.h"

void Logger::log(int ret, char *errorPlace, int errno)
{
    if (ret == -1 && debug) {
        std::cerr << "Error at " << errorPlace << "Errno: " << errno << std::endl;
    }
}