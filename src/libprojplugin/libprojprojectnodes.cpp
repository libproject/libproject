#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>

using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using ProjectExplorer::ProjectAction;

namespace  LibprojManager {
namespace Internal {

ProjectNode::ProjectNode(Project * Project, ProjectFile * ProjectFile)
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
    Q_UNUSED(node);
    return QList<ProjectAction>()
        << ProjectExplorer::ProjectAction::AddNewFile;
}

/*dummies*/
bool ProjectNode::canAddSubProject(const QString &proFilePath) const
{
    qDebug() << "Calling dummy ProjectNode::canAddSubProject()";
    return false;
}
bool ProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "Calling dummy ProjectNode::addSubProjects()";
    return false;
}
bool ProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "Calling dummy ProjectNode::removeSubProjects()";
    return false;
}
/*dummies*/

bool ProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    qDebug() << "Calling ProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)";
    Q_UNUSED(notAdded)
    return project->addFiles(filePaths);
}

} // namespace Internal
} // namespace LibprojManager
