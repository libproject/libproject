#pragma once
#include <projectexplorer/projectnodes.h>

namespace ProjectExplorer {
class Project;
}
namespace Core {
class IDocument;
}
namespace LibprojManager {
namespace Internal {

class Project;
class ProjectFile;
class ProjectNode : public ProjectExplorer::ProjectNode {
  ProjectExplorer::Project *project;
  ProjectFile *projectFile;

public:
  ProjectNode(ProjectExplorer::Project *Project, ProjectFile *ProjectFile);
  QList<ProjectExplorer::ProjectAction> supportedActions(Node *node) const;
  virtual bool canAddSubProject(const QString &proFilePath) const;
  virtual bool addSubProjects(const QStringList &proFilePaths);
  virtual bool removeSubProjects(const QStringList &proFilePaths);

  bool addFiles(const QStringList &filePaths, QStringList *notAdded);
  const QString getProjectPath() const;
};

} // namespace Internal
} // namespace LibprojManager
