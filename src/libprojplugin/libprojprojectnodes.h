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
    OwnProjectNode(std::shared_ptr<OwnProject> Project, std::shared_ptr<OwnProjectFile> ProjectFile); //1st
    OwnProjectNode(OwnProject * Project, std::shared_ptr<OwnProjectFile> ProjectFile); //2nd
public /*dummies*/:
    virtual bool canAddSubProject(const QString &proFilePath) const;
    virtual bool addSubProjects(const QStringList &proFilePaths);
    virtual bool removeSubProjects(const QStringList &proFilePaths);
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTNODE_H
