#pragma once
#include "libprojconstants.h"
#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include <string>

namespace LibprojManager {
namespace Interface {

class FileSetLoader;
class AbstractProjectCreator {
public:
  virtual ~AbstractProjectCreator() {}

  virtual ProjectExplorer::Project *
  create(LibprojManager::Internal::Manager *Manager_,
         const FileSetLoader *Loader_);
};

class ProjectCreator : public AbstractProjectCreator {
public:
  virtual ~ProjectCreator() {}

  virtual ProjectExplorer::Project *
  create(LibprojManager::Internal::Manager *Manager_,
         const FileSetLoader *Loader_) const {
    qDebug() << "Creating project by factory method";
    return new LibprojManager::Internal::Project(Manager_, Loader_);
  }
};

class ProjectFactory {
protected:
  static AbstractProjectCreator *creator;

public:
  ~ProjectFactory();

  static ProjectExplorer::Project *
  createProject(LibprojManager::Internal::Manager *Manager__,
                const FileSetLoader *Loader__) {
    return creator->create(Manager__, Loader__);
  }
};

} // namespace Interface
} // namespace LibprojManager
