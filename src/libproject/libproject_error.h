#pragma once

#include <stdexcept>

namespace LibprojManager {
namespace Interface {
namespace Error {

/*!
 * \brief Class which represents runtime error in FileSet interface
 *
 * Instances of this class should generates by throwing
 * mechanizm when FileSet interface getting
 * an erroneous state of runtime kind. ErrType represents type of
 * runtime error
 */
class FileSetRuntimeError : public std::runtime_error {
public:
    enum ErrType {
        UnknownError = 0,
        IncorrectSource,
        AlreadyLoaded
    };

    FileSetRuntimeError(const ErrType type, const std::string& message)
        : runtime_error(message), errorType(type) { }

    const ErrType getErrorType() const { return errorType; }
private:
    ErrType errorType;
};

} // namespace Error
} // namespace Interface
} // namespace LibprojManager
