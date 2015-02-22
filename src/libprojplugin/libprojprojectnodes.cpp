#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>

using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::LibprojPlugin;
using ProjectExplorer::ProjectAction;

namespace  LibprojProjectManager {
namespace Internal {

OwnProjectNode::OwnProjectNode(OwnProject * Project, OwnProjectFile * ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath().toString()),
      project(Project),
      projectFile(ProjectFile)
{
    qDebug() << "Calling c-tor for OwnProjectNode";
    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());
}

QList<ProjectExplorer::ProjectAction> OwnProjectNode::supportedActions(Node *node) const
{
    qDebug() << "Calling OwnProjectNode::supportedActions(Node *node) const";
    Q_UNUSED(node);
    return QList<ProjectAction>()
        << ProjectExplorer::ProjectAction::AddNewFile;
}

/*dummies*/
bool OwnProjectNode::canAddSubProject(const QString &proFilePath) const
{
    qDebug() << "Calling dummy OwnProjectNode::canAddSubProject()";
    return false;
}
bool OwnProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "Calling dummy OwnProjectNode::addSubProjects()";
    return false;
}
bool OwnProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "Calling dummy OwnProjectNode::removeSubProjects()";
    return false;
}
/*dummies*/

bool OwnProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    qDebug() << "Calling OwnProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)";
    Q_UNUSED(notAdded)
    return project->addFiles(filePaths);
}

} // namespace Internal
} // namespace LibprojProjectManager
