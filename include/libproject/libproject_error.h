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
        UnknownError,
        IncorrectSource,
        ProjectAlreadyLoaded,
        NotLoadedInterface,
        AddBrokenSubproject,
        LoadBrokenSubproject,
        SubprojectsIncongruity
    };

    FileSetRuntimeError(const ErrType type, const std::string& message = std::string())
        : runtime_error(message), errorType(type) { }

    ErrType getErrorType() const { return errorType; }

    const std::string toString() const
    {
        switch (errorType)
        {
        case UnknownError:
            return std::string("Unknown Error\nSee additional info through what() function");
        case IncorrectSource:
            return std::string("Encountered problem with source\nSee additional info through what() function");
        case ProjectAlreadyLoaded:
            return std::string("Loader have loaded project already");
        case NotLoadedInterface:
            return std::string("Loader haven\'t loaded project");
        case AddBrokenSubproject:
            return std::string("There was try to add broken subproject");
        case LoadBrokenSubproject:
            return std::string("There was try to load broken subproject");
        case SubprojectsIncongruity:
            return std::string("Some incongruity detected\nSee additional info through what() function");
        }
    }
private:
    ErrType errorType;
};

} // namespace Error
} // namespace Interface
} // namespace LibprojManager
