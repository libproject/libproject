#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
using std::shared_ptr;
namespace  LibprojProjectManager {
namespace Internal {

OwnProjectNode::OwnProjectNode(shared_ptr<OwnProject> Project, shared_ptr<OwnProjectFile> ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath().toString()), project(Project),
      projectFile(ProjectFile)
{
    qDebug() << "[dbg]\t\tCalling c-tor for OwnProjectNode";
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

} // namespace Internal
} // namespace LibprojProjectManager
