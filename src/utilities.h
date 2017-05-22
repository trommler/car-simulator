/** 
 * @file utilities.h
 * @author Florian Bauer
 *
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace utils {
  bool existsFile(const std::string& filepath) noexcept;
  bool existsDirectory(const std::string& dirpath) noexcept;
}

#endif /* UTILITIES_H */