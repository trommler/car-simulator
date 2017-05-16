/** 
 * @file Utilities.h
 * @author Florian Bauer
 *
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace utils
{
    bool existsFile(const std::string& filePath);
    bool existsDirectory(const std::string& dirPath);
}

#endif /* UTILITIES_H */