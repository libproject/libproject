#pragma once
#include "libprojplugin.h"
#include <projectexplorer/project.h>
#include "json11.hpp"

namespace LibprojManager {
namespace Internal {

class Manager;
class ProjectFile;
class ProjectNode;
class Project : public ProjectExplorer::Project
{
    Q_OBJECT
    Manager * manager;
    QString filename, nameOfProject;
    ProjectFile * file;
    ProjectNode * rootNode;
    json11::Json projectData;

public:
    Project(Manager * Manager, const QString & Filename, const QString & ContentOfProjectFile);

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

    friend class Libproj::Internal::Plugin;
};

} // namespace Internal
} // namespace LibprojManager
