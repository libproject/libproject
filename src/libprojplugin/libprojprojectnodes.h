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
    OwnProjectFile * projectFile; //unique?
public:
    OwnProjectNode(OwnProject * Project, OwnProjectFile * ProjectFile); //2nd
public /*dummies*/:
    virtual bool canAddSubProject(const QString &proFilePath) const;
    virtual bool addSubProjects(const QStringList &proFilePaths);
    virtual bool removeSubProjects(const QStringList &proFilePaths);

public /*my*/:
    void addFileNodes(const QVariantMap& Data, const QFileInfo &fileInfo);
};

} // namespace Internal
} // namespace LibprojProjectManager
