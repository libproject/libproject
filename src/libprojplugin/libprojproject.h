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
    Interface::FileSetLoader * loader;
    Manager * manager;
    QString nameOfProject;
    QString pathToNode;
    QStringList projectFiles;
    ProjectFile * projectFile;
    ProjectNode * rootNode;

    QList<ProjectExplorer::ProjectNode *> subprojectNodes;
    friend bool ProjectNode::removeSubProjects(const QStringList &proFilePaths);

public:
    Project(Manager * Manager, Interface::FileSetLoader *Loader);

    QString displayName() const;
    const QString getPathToNode() const { return pathToNode; }
    Core::IDocument * document() const;
    ProjectExplorer::IProjectManager * projectManager() const;
    ProjectExplorer::ProjectNode * rootProjectNode() const;
    ProjectFile * getProjectFile() const;
    QStringList files(FilesMode fileMode) const;
    QStringList files() const;
    bool addFiles(const QStringList & filePaths);
    const QList<ProjectExplorer::ProjectNode *> & getSubprojectNodes() const { return subprojectNodes; }
    Interface::FileSetLoader * getLoader() { return loader; }
};

} // namespace Internal
} // namespace LibprojManager
