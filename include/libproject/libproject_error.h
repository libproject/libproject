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
        NotSpecified,
        ProjectAlreadyLoaded,
        ProjectNotLoaded,
        AddBrokenSubproject,
        LoadBrokenSubproject,
        SubprojectsIncongruity
    };

    FileSetRuntimeError(ErrType type, const std::string& message = std::string())
        : runtime_error(message) { }

    ErrType getErrorType() const { return errorType; }
    virtual std::string errorDescription() const = 0;
protected:
    ErrType errorType;
};

class UnknownRuntimeError : public FileSetRuntimeError {
public:
    UnknownRuntimeError(const std::string& message = std::string())
        : FileSetRuntimeError(NotSpecified, message) { }

    std::string errorDescription() const
    {
        return std::string("Unknown Error\nSee additional info through what() function");
    }
};

class IncorrectSourceError : public FileSetRuntimeError {
public:
    IncorrectSourceError(const std::string& message = std::string())
        : FileSetRuntimeError(NotSpecified, message) { }

    std::string errorDescription() const
    {
        return std::string("Encountered problem with source\nSee additional info through what() function");
    }
};

class LoaderStateError : public FileSetRuntimeError {
public:
    LoaderStateError(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }

    std::string errorDescription() const
    {
        switch (errorType)
        {
        case ProjectAlreadyLoaded:
            return std::string("Loader have loaded project already\nSee additional info through what() function");
        case ProjectNotLoaded:
            return std::string("Loader haven\'t loaded project\nSee additional info through what() function");
        }
    }
};

class SubprojectsError : public FileSetRuntimeError {
public:
    SubprojectsError(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }

    std::string errorDescription() const
    {
        switch (errorType)
        {
        case AddBrokenSubproject:
            return std::string("There was try to add broken subproject\nSee additional info through what() function");
        case LoadBrokenSubproject:
            return std::string("There was try to load broken subproject\nSee additional info through what() function");
        case SubprojectsIncongruity:
            return std::string("Some incongruity detected\nSee additional info through what() function");
        }
    }
};


} // namespace Error
} // namespace Interface
} // namespace LibprojManager

// x в FileSetRuntimeError убрать второй аргумент
// ? создать абстрактную функцию ErrDescr() в FileSetRuntimeError
//породить классы от FileSetRuntimeError
