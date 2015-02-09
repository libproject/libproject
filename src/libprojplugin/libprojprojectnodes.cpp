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
    qDebug() << "[dbg]\t\tCalling c-tor for OwnProjectNode (2nd)";
    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());
}

/*dummies*/
bool OwnProjectNode::canAddSubProject(const QString &proFilePath) const
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectNode::canAddSubProject()";
    return false;
}
bool OwnProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectNode::addSubProjects()";
    return false;
}
bool OwnProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectNode::removeSubProjects()";
    return false;
}
/*dummies*/

/*my*/
void OwnProjectNode::addFileNodes(const QVariantMap & Data, const QFileInfo & fileInfo)
{
   for (const auto& x : Data["files"].toStringList())
   {
       LibprojPlugin::files.push_back(new QFile(fileInfo.dir().path() + QString("\\") + x));
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
