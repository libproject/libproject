#pragma once

#include <projectexplorer/iprojectmanager.h>

namespace LibprojProjectManager {
namespace Internal {

class OwnProject;
class OwnManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT
    OwnProject * project;
public:
    OwnManager();

    virtual QString mimeType() const;
    virtual ProjectExplorer::Project * openProject(const QString &Filename, QString *ErrorString);

    void registerProject(OwnProject * Project);
    void unregisterProject(OwnProject * /*Project*/);
};

} // namespace Internal
} // namespace LibprojProjectManager
