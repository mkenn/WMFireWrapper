#include "util.h"
#include <sys/stat.h>

bool FileExists(char const* filename)
{
    struct stat buffer;
    return 0 == stat(filename, &buffer);
}


