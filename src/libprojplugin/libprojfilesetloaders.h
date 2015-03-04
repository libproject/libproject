#pragma once
#include <list>
#include <string>
#include "json11.hpp"

namespace LibprojManager {
namespace Interface {

class FileSetLoader
{
public:

    virtual ~FileSetLoader() { }

    virtual bool open() = 0;
    virtual const std::list<std::string> getFileNames() const = 0;
    virtual const std::string getProjectName() const = 0;
    virtual const std::string getPathToRootNode() const = 0;
};


class JsonFileSetLoader : public FileSetLoader
{

    std::string sContentOfProjectFile,
                pathToProjectFile;
    json11::Json jContentOfProjectFile;
    bool loaded;

public:
    JsonFileSetLoader(const std::string& ProjectFile)
        : pathToProjectFile(ProjectFile), loaded(false) { }

    virtual bool open();
    virtual const std::list<std::string> getFileNames() const;
    virtual const std::string getProjectName() const;
    virtual const std::string getPathToRootNode() const { return loaded ? pathToProjectFile : std::string(); }

};

class AbstractFileSetCreator {
public:

    virtual ~AbstractFileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const = 0;
};

class FileSetCreator : public AbstractFileSetCreator {
public:

    virtual ~FileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const {
        return new JsonFileSetLoader(pathToRootNode);
    }
};

class FileSetFactory
{
protected:
    class AbstractFileSetCreatorSingleton
    {
    private:
        AbstractFileSetCreatorSingleton() { }
    public:

        AbstractFileSetCreatorSingleton
        (const AbstractFileSetCreatorSingleton&) = delete;
        AbstractFileSetCreatorSingleton&
        operator=(AbstractFileSetCreatorSingleton&) = delete;

        static AbstractFileSetCreator& getCreator() {
            static FileSetCreator creator;
            return creator;
        }
    };

public:

    static FileSetLoader * createFileSet(const std::string & pathToRootNode_)
    {
        return AbstractFileSetCreatorSingleton::getCreator().create(pathToRootNode_);
    }
};



} // namespace Interface
} // namespace LibprojManager
