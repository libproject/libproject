#pragma once

#include <gmock/gmock.h>
#include "fileset.h"

namespace LibprojManager {
namespace Interface {

class FileSetLoaderMock : public FileSetLoader {

public:
    MOCK_METHOD0(open, bool());
    MOCK_CONST_METHOD0(getFileNames, const std::list<std::string>());
    MOCK_CONST_METHOD0(getProjectName, const std::string());
    MOCK_CONST_METHOD0(getPathToRootNode, const std::string());
};


} // namespace Interface
} // namespace LibprojManager
