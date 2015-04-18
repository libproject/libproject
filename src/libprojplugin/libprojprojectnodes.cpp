#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>
#include "libprojprojectmanager.h"

typedef ProjectExplorer::Project AbstractProject;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using ProjectExplorer::ProjectAction;
using LibprojManager::Internal::Project;

namespace  LibprojManager {
namespace Internal {

ProjectNode::ProjectNode(AbstractProject * Project, ProjectFile * ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath().toString()),
      project(Project),
      projectFile(ProjectFile)
{
    qDebug() << "Calling c-tor for ProjectNode";
    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());
}

QList<ProjectExplorer::ProjectAction> ProjectNode::supportedActions(Node *node) const
{
    qDebug() << "Calling ProjectNode::supportedActions(Node *node) const";
    //Q_UNUSED(node);
    return QList<ProjectAction>()
        << ProjectExplorer::ProjectAction::AddNewFile
        << ProjectExplorer::ProjectAction::AddSubProject
        << ProjectExplorer::ProjectAction::RemoveSubProject;
}

bool ProjectNode::canAddSubProject(const QString &proFilePath) const
{
    if (QFileInfo(proFilePath).suffix() == QString("libproject"))
        return true;
    return false;
}
bool ProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    return qobject_cast<Project*>(project)->addFiles(proFilePaths);
}
bool ProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    //Manager * m = qobject_cast <Manager *>(project->projectManager());
    removeProjectNodes(qobject_cast<Project *>(project)->getSubprojectNodes());
    return true; //Err check TODO
}

bool ProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    qDebug() << "Calling ProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)";
    Q_UNUSED(notAdded)
    return qobject_cast<Project*>(project)->addFiles(filePaths);
}

} // namespace Internal
} // namespace LibprojManager
