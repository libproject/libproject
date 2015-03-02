#pragma once
#include "libprojconstants.h"
#include "libprojinterface.h"
#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include <array>
#include <string>

typedef std::array<QString, 2> paths;

namespace LibprojManager {
namespace Interface {

class AbstractProjectCreator {
public:

    virtual ~AbstractProjectCreator() { }

    virtual ProjectExplorer::Project * create(LibprojManager::Internal::Manager * Manager_,
                                              const std::string & ContentOfProjectFile_,
                                              const paths& Paths_) const = 0;
};

class ProjectCreator : public AbstractProjectCreator {
public:

    virtual ~ProjectCreator() { }

    virtual ProjectExplorer::Project * create(LibprojManager::Internal::Manager * Manager_,
                                              const std::string & ContentOfProjectFile_,
                                              const paths& Paths_) const {
        qDebug() << "Creating project by factory method";
        return new LibprojManager::Internal::Project(Manager_, ContentOfProjectFile_, Paths_);
    }
};

class ProjectFactory
{
protected:
    static AbstractProjectCreator * creator;
public:

    ~ProjectFactory();

    static ProjectExplorer::Project * createProject(LibprojManager::Internal::Manager * Manager__,
                                      const std::string & ContentOfProjectFile__,
                                      const paths& Paths__)
    {
        return creator->create(Manager__,
                                  ContentOfProjectFile__,
                                  Paths__);
    }

};

} // namespace Interface
} // namespace LibprojManager
