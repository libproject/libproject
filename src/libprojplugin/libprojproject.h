#pragma once
#include "libprojplugin.h"
#include <projectexplorer/project.h>
#include "libprojinterface.h"


namespace LibprojManager {
namespace Internal {

class Manager;
class ProjectFile;
class ProjectNode;
class Project : public ProjectExplorer::Project, protected Interface::InputInterface
{
    Q_OBJECT
    Manager * manager;
    QString nameOfProject;
    QStringList projectFiles;
    ProjectFile * projectFile;
    ProjectNode * rootNode;

public:
    Project(Manager * Manager, QFile &MainFile);

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;
    QStringList files() const;
    bool addFiles(const QStringList &filePaths);

protected /*interface*/:
    virtual bool readFile(QFile &ProjectFile);
    virtual const QStringList getFileNames() const;
    QVariantMap jsonToQVariantMap(const json11::Json& json) const;
    json11::Json contentOfProjectFile;

    friend class Libproj::Internal::Plugin;
};

} // namespace Internal
} // namespace LibprojManager
