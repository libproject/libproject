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
    /*std::list<*/OwnProjectFile * /*>*/file/*s*/;
    OwnProjectNode * rootNode;

public:
    OwnProject(OwnManager * Manager, const QString & Filename); //2nd

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

};

} // namespace Internal
} // namespace LibprojProjectManager
