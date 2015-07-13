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
        SourceAPIError,
        SourceError,
        InputStreamError,
        OutputStreamError,
        ProjectAlreadyLoaded,
        GetPathToRootNodeOnNotLoaded,
        GetFileNamesOnNotLoaded,
        GetProjectNameOnNotLoaded,
        GetSubprojectsPathsOnNotLoaded,
        GetSubprojectsLoadersOnNotLoaded,
        AddSubprojectsOnNotLoaded,
        RemoveSubprojectsOnNotLoaded,
        CountSubprojectsOnNotLoaded,
        AddExistingSubproject,
        AddEqualSubprojects,
        AddBrokenSubproject,
        LoadBrokenSubproject,
        RemoveNonExistentSubproject,
        GetSubprojectsPathWhereThereAreNoneOfThem,
        FoundDuplicateInCandidatesToRemove,
        EmptySubprojectsContainerDetected,
        UnknownError
    };

    FileSetRuntimeError(ErrType type, const std::string& message)
        : runtime_error(message), errorType(type) { }

    //ErrType getErrorType() const { return errorType; }
protected:
    ErrType errorType;
};

class IncorrectLoaderBehaviour : public FileSetRuntimeError {
public:
    IncorrectLoaderBehaviour(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }

};

class IncorrectSourceError : public FileSetRuntimeError {
public:
    IncorrectSourceError(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }
};

class LoaderStateError : public FileSetRuntimeError {
public:
    LoaderStateError(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }
};

class SubprojectsError : public FileSetRuntimeError {
public:
    SubprojectsError(ErrType type, const std::string& message = std::string())
        : FileSetRuntimeError(type, message) { }

};


} // namespace Error
} // namespace Interface
} // namespace LibprojManager

// x в FileSetRuntimeError убрать второй аргумент
// ? создать абстрактную функцию ErrDescr() в FileSetRuntimeError
//породить классы от FileSetRuntimeError
