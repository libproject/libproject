#pragma once
#include <projectexplorer/projectnodes.h>

namespace Core { class IDocument; }
namespace  LibprojProjectManager {
namespace Internal {

class OwnProject;
class OwnProjectFile;
class OwnProjectNode : public ProjectExplorer::ProjectNode
{
    OwnProject * project;
    OwnProjectFile * projectFile;
public:
    OwnProjectNode(OwnProject * Project, OwnProjectFile * ProjectFile);
    QList<ProjectExplorer::ProjectAction> supportedActions(Node * node) const;
public /*dummies*/:
    virtual bool canAddSubProject(const QString &proFilePath) const;
    virtual bool addSubProjects(const QStringList &proFilePaths);
    virtual bool removeSubProjects(const QStringList &proFilePaths);

public:
    bool addFiles(const QStringList &filePaths, QStringList *notAdded);
};

} // namespace Internal
} // namespace LibprojProjectManager
