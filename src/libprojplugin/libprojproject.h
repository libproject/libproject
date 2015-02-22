#pragma once
#include "libprojplugin.h"
#include <projectexplorer/project.h>
#include "json11.hpp"

namespace LibprojProjectManager {
namespace Internal {

class OwnManager;
class OwnProjectFile;
class OwnProjectNode;
class OwnProject : public ProjectExplorer::Project
{
    Q_OBJECT
    OwnManager * manager;
    QString filename, nameOfProject;
    OwnProjectFile * file;
    OwnProjectNode * rootNode;
    json11::Json projectData;

public:
    OwnProject(OwnManager * Manager, const QString & Filename, const QString & ContentOfProjectFile);

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;
    QStringList files() const;
    bool addFiles(const QStringList &filePaths);

private:
    json11::Json getProjectData() const { return projectData; }
    json11::Json setProjectData(const json11::Json& NewProjectData)  { projectData = NewProjectData; }
    QVariantMap jsonToQVariantMap(const json11::Json& json) const;

    friend class Libproj::Internal::LibprojPlugin; /*::
    saveProjectData(const std::string &, const std::string &);*/
};

} // namespace Internal
} // namespace LibprojProjectManager
