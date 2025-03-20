#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <iostream>
#include <fstream>
#include <string>

namespace MiscUtils {
std::string mkTmp();
std::string writeStringToTmpFile(const std::string& str);
};

#endif // MISC_UTILS_H
