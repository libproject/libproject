#pragma once
#include <projectexplorer/project.h>

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

public:
    OwnProject(OwnManager * Manager, const QString & Filename); //2nd

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

    /*LPROJ-8*/
     QStringList files() const;
      bool addFiles(const QStringList &filePaths);
};

} // namespace Internal
} // namespace LibprojProjectManager
