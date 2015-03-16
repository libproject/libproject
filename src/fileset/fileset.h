#pragma once

#include <list>
#include <string>

namespace LibprojManager {
namespace Interface {

class FileSetLoader
{
public:

    virtual ~FileSetLoader() { }

    virtual const std::string open() = 0;
    virtual const std::list<std::string> getFileNames() const = 0;
    virtual const std::string getProjectName() const = 0;
    virtual const std::string getPathToRootNode() const = 0;

};


class AbstractFileSetCreator {
public:

    virtual ~AbstractFileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const = 0;
};

class FileSetCreator : public AbstractFileSetCreator {
public:

    virtual ~FileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const;
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

    static FileSetLoader * createFileSet(const std::string & pathToRootNode_);
};



} // namespace Interface
} // namespace LibprojManager
