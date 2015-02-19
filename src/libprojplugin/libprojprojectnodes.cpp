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

void OwnProjectNode::addFileNodes(const QVariantMap & Data, const QFileInfo & fileInfo)
{
    qDebug() << "Calling OwnProjectNode::addFileNodes(const QVariantMap & Data, const QFileInfo & fileInfo)";
    QList<FileNode*> listOfFileNodes;
    listOfFileNodes.push_back(new FileNode(fileInfo.absoluteFilePath(), FileType::ProjectFileType,  false/**/));
   for (const auto& x : Data["files"].toStringList())
   {
       LibprojPlugin::files.push_back(new QFile(fileInfo.dir().path() + QString("/") + x));
       LibprojPlugin::files.last()->open( QIODevice::ReadWrite | QIODevice::Text );
   }
   for (const auto& x : LibprojPlugin::files)
       listOfFileNodes.push_back(new FileNode(QFileInfo(*x).absoluteFilePath(), FileType::SourceType, false));
   this->ProjectNode::addFileNodes(listOfFileNodes);
}

bool OwnProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    qDebug() << "Calling OwnProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)";
    Q_UNUSED(notAdded)
    return project->addFiles(filePaths);
}

} // namespace Internal
} // namespace LibprojProjectManager
