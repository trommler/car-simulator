/** 
 * @file Utilities.cpp
 * @author Florian Bauer
 * 
 * This file offers utility functions in the `utils` namespace for common used file system
 * operations.
 */

#include "Utilities.h"
#include <sys/stat.h>
#include <cstring>

/**
 * Checks if a file exists on the file system.
 * 
 * @param filePath: the path to the file as string
 * @return true if file exists, otherwise false
 */
bool utils::existsFile(const std::string& filePath)
{
    struct stat64 sb;

    if (stat64(filePath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
    {
        return true;
    }
    return false;
}

/**
 * Checks if a directory exists on the file system.
 * 
 * @param dirPath: the path to the directory as string
 * @return true if directory exists, otherwise false
 */
bool utils::existsDirectory(const std::string& dirPath)
{
    struct stat64 sb;

    if (stat64(dirPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }
    return false;
}