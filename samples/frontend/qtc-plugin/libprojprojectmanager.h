#pragma once
#include <projectexplorer/iprojectmanager.h>

namespace ProjectExplorer { class Project; }
namespace LibprojManager {
namespace Internal {

class Manager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT
    QList<ProjectExplorer::Project *> projects;
public:
    Manager();

    virtual QString mimeType() const;
    virtual ProjectExplorer::Project * openProject(const QString & Filename, QString * ErrorString);

    void registerProject(ProjectExplorer::Project * Project);
    void unregisterProject(ProjectExplorer::Project * Project);
};

} // namespace Internal
} // namespace LibprojManager
