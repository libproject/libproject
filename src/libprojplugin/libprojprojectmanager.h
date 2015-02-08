#ifndef LIBPROJPROJECTMANAGER_H
#define LIBPROJPROJECTMANAGER_H
#include <projectexplorer/iprojectmanager.h>
#include <memory>

namespace LibprojProjectManager {
namespace Internal {

class OwnProject;
class OwnManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT
    std::shared_ptr<OwnProject> project;
public:
    OwnManager();

    virtual QString mimeType() const;
    virtual ProjectExplorer::Project * openProject(const QString &Filename, QString *ErrorString);

};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTMANAGER_H
