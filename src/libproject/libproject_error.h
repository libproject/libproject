#pragma once

#include <stdexcept>

namespace LibprojManager {
namespace Interface {
namespace Error {

/*!
 * \brief Class which represents logic error in FileSet interface
 *
 * Instances of this class appears when FileSet interface getting
 * an erroneous state of logic kind. ErrType represents type of
 * logic error
 */
class FileSetLogicError : public std::logic_error {
public:
    enum ErrType {
        UnknownError = 0,
        AlreadyLoaded
    };

    FileSetLogicError(const ErrType type, const std::string& message)
        : logic_error(message), errorType(type) { }
private:
    ErrType errorType;
};

/*!
 * \brief Class which represents runtime error in FileSet interface
 *
 * Instances of this class appears when FileSet interface getting
 * an erroneous state of runtime kind. ErrType represents type of
 * runtime error
 */
class FileSetRuntimeError : public std::runtime_error {
public:
    enum ErrType {
        UnknownError = 0,
        IncorrectSource
    };

    FileSetRuntimeError(const ErrType type, const std::string& message)
        : runtime_error(message), errorType(type) { }
private:
    ErrType errorType;
};

} // namespace Error
} // namespace Interface
} // namespace LibprojManager
