#include "MiscUtils.h"
#include "Logger.h"
#include <unistd.h>

namespace MiscUtils {
std::string mkTmp() {
    char tempFile[] = "/tmp/fakeRestData-XXXXXX";
    int fd = mkstemp(tempFile);
    if (fd == -1) {
        return "";
    }
    close(fd);
    return std::string(tempFile);
}

std::string writeStringToTmpFile(std::string& str) {
    std::string tmpFile = mkTmp();
    std::ofstream output_file(tmpFile);
    if (output_file.is_open()) {
        output_file << str << std::endl;
        output_file.close();
        return tmpFile;
    }
    return "";
}

} // namespace MiscUtils
