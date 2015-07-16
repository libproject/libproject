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

    FileSetRuntimeError(const std::string& message)
        : runtime_error(message) { }
};

class IncorrectLoaderBehaviour : public FileSetRuntimeError {
public:
    enum ErrorType {
        EmptySubprojectsContainerDetected,
        FoundDuplicateInCandidatesToRemove,
        UnknownError
    };

    IncorrectLoaderBehaviour(ErrorType type, const std::string& message = std::string())
        : FileSetRuntimeError(message), errorType(type) { }

private:
    ErrorType errorType;

};

class IncorrectSourceError : public FileSetRuntimeError {
public:
    enum ErrorType {
        SourceError
    };

    IncorrectSourceError(ErrorType type, const std::string& message = std::string())
        : FileSetRuntimeError(message), errorType(type) { }

private:
    ErrorType errorType;
};

class LoaderStateError : public FileSetRuntimeError {
public:
    enum ErrorType {
        LoadProjectWhileAlreadyLoaded,
        GetPathToRootNodeWhileNotLoaded,
        GetFileNamesWhileNotLoaded,
        GetProjectNameWhileNotLoaded,
        GetSubprojectsPathsWhileNotLoaded,
        CountSubprojectsWhileNotLoaded,
        GetSubprojectsLoadersWhileNotLoaded,
        AddSubprojectsWhileNotLoaded,
        RemoveSubprojectsWhileNotLoaded
    };

    LoaderStateError(ErrorType type, const std::string& message = std::string())
        : FileSetRuntimeError(message), errorType(type) { }

private:
    ErrorType errorType;
};

class SubprojectsError : public FileSetRuntimeError {
public:
    enum ErrorType {
        GetSubprojectsPathsWhileThereAreNoSubprojects,
        AddBrokenSubproject,
        AddExistingSubproject,
        AddEqualSubprojects,
        RemoveNonExistentSubproject,
        LoadBrokenSubproject

    };

    SubprojectsError(ErrorType type, const std::string& message = std::string())
        : FileSetRuntimeError(message), errorType(type) { }

private:
    ErrorType errorType;
};

class StreamError : public FileSetRuntimeError {
public:
    enum ErrorType {
        InputStreamError,
        OutputStreamError,
        SourceAPIError
    };

    StreamError(ErrorType type, const std::string& message = std::string())
        : FileSetRuntimeError(message), errorType(type) { }

private:
    ErrorType errorType;
};


} // namespace Error
} // namespace Interface
} // namespace LibprojManager

