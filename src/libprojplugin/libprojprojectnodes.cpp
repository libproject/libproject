#pragma once

#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>

using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::LibprojPlugin;

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

/*my*/
void OwnProjectNode::addFileNodes(const QVariantMap & Data, const QFileInfo & fileInfo)
{
   for (const auto& x : Data["files"].toStringList())
   {
       LibprojPlugin::files.push_back(new QFile(fileInfo.dir().path() + QString("/") + x));
       LibprojPlugin::files.last()->open( QIODevice::ReadWrite | QIODevice::Text );
   }
   QList<FileNode*> listOfFileNodes;
   for (const auto& x : LibprojPlugin::files)
       listOfFileNodes.push_back(new FileNode(QFileInfo(*x).absoluteFilePath(), FileType::SourceType, false));
   this->ProjectNode::addFileNodes(listOfFileNodes);
}

/*my*/

} // namespace Internal
} // namespace LibprojProjectManager
