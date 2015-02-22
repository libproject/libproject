#pragma once
#include <projectexplorer/iprojectmanager.h>

namespace LibprojManager {
namespace Internal {

class Project;
class Manager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT
    Project * project;
public:
    Manager();

    virtual QString mimeType() const;
    virtual ProjectExplorer::Project * openProject(const QString &Filename, QString *ErrorString);

    void registerProject(Project * Project);
    void unregisterProject(Project * /*Project*/);

private:
    QString readProjectFile(const QString& Filename) const;
};

} // namespace Internal
} // namespace LibprojManager
