#pragma once
#include <projectexplorer/projectnodes.h>

namespace Core { class IDocument; }
namespace  LibprojManager {
namespace Internal {

class Project;
class ProjectFile;
class ProjectNode : public ProjectExplorer::ProjectNode
{
    Project * project;
    ProjectFile * projectFile;
public:
    ProjectNode(Project * Project, ProjectFile * ProjectFile);
    QList<ProjectExplorer::ProjectAction> supportedActions(Node * node) const;
public /*dummies*/:
    virtual bool canAddSubProject(const QString &proFilePath) const;
    virtual bool addSubProjects(const QStringList &proFilePaths);
    virtual bool removeSubProjects(const QStringList &proFilePaths);

public:
    bool addFiles(const QStringList &filePaths, QStringList *notAdded);
};

} // namespace Internal
} // namespace LibprojManager
