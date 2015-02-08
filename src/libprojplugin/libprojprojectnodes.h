#ifndef LIBPROJPROJECTNODE_H
#define LIBPROJPROJECTNODE_H

#include <projectexplorer/projectnodes.h>
#include <memory>

namespace Core { class IDocument; }

namespace  LibprojProjectManager {
namespace Internal {

class OwnProject;
class OwnProjectFile;
class OwnProjectNode : public ProjectExplorer::ProjectNode
{
    std::shared_ptr<OwnProject> project;
    std::shared_ptr<OwnProjectFile> projectFile; //unique?
public:
    OwnProjectNode(std::shared_ptr<OwnProject> Project, std::shared_ptr<OwnProjectFile> ProjectFile);
public /*dummies*/:
    virtual bool canAddSubProject(const QString &proFilePath) const { return false; }
    virtual bool addSubProjects(const QStringList &proFilePaths) { return false; }
    virtual bool removeSubProjects(const QStringList &proFilePaths) { return false; }
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTNODE_H
