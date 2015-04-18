#pragma once
#include "libprojplugin.h"
#include <projectexplorer/project.h>
#include <string>
#include <vector>
#include <array>
#include "libprojconstants.h"

namespace LibprojManager {
namespace Interface { class FileSetLoader; }
namespace Internal {

class Manager;
class ProjectFile;
class ProjectNode;

class Project : public ProjectExplorer::Project
{
    Q_OBJECT
    Manager * manager;
    QString nameOfProject;
    QStringList projectFiles;
    ProjectFile * projectFile;
    ProjectNode * rootNode; //ProjectExplorer::
    QList<ProjectExplorer::ProjectNode *> subprojectNodes;

public:
    Project(Manager * Manager, const Interface::FileSetLoader * Loader);

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    ProjectFile * getProjectFile() const;
    QStringList files(FilesMode fileMode) const;
    QStringList files() const;
    bool addFiles(const QStringList &filePaths);
    /*const*/QList<ProjectExplorer::ProjectNode *>/*&*/ getSubprojectNodes() const { return subprojectNodes; }
};

} // namespace Internal
} // namespace LibprojManager
