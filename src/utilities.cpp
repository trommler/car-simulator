/** 
 * @file utilities.cpp
 * @author Florian Bauer
 * 
 * This file offers utility functions in the `utils` namespace for common used
 * file system operations.
 */

#include "utilities.h"
#include <sys/stat.h>
#include <cstring>

/**
 * Checks if a file exists on the file system.
 * 
 * @param filepath: the path to the file as string
 * @return true if file exists, otherwise false
 */
bool utils::existsFile(const std::string& filepath) noexcept
{
    struct stat64 sb;

    if (stat64(filepath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
    {
        return true;
    }
    return false;
}

/**
 * Checks if a directory exists on the file system.
 * 
 * @param dirpath: the path to the directory as string
 * @return true if directory exists, otherwise false
 */
bool utils::existsDirectory(const std::string& dirpath) noexcept
{
    struct stat64 sb;

    if (stat64(dirpath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }
    return false;
}