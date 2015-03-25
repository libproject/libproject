#pragma once

#include <stdexcept>

namespace LibprojManager {
namespace Interface {
namespace Error {

enum Code {
    /* logic errors */
    UNKL = 0,   //unknown logic error
    PRLD,       //project already loaded

    /* runtime errors */
    UNKR = 100, //unknown runtime error
    ICRS        //incorrect sources
};

class ErrorInLoaderLogic : public std::logic_error {
    Code errorCode;
public:
    ErrorInLoaderLogic(const Code code, const std::string& message)
        : logic_error(message), errorCode(code) { }
};

class ErrorDuringGettingProjectInfo : public std::runtime_error {
    Code errorCode;
public:
    ErrorDuringGettingProjectInfo(const Code code, const std::string& message)
        : runtime_error(message), errorCode(code) { }
};

} // namespace Error
} // namespace Interface
} // namespace LibprojManager
