#include "../includes/Logger.h"

bool Logger::debug = false;

void Logger::log(int ret, char *errorPlace, int errno)
{
    if (ret == -1 && Logger::debug) {
        std::cerr << "Error at " << errorPlace << "Errno: " << errno << std::endl;
    }
}
