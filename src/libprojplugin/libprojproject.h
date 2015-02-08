#ifndef LIBPROJPROJECT_H
#define LIBPROJPROJECT_H
#include <projectexplorer/project.h>
#include <memory>

namespace LibprojProjectManager {

namespace Internal {
class OwnManager;
class OwnProjectFile;
}

class OwnProject : public ProjectExplorer::Project
{
    Q_OBJECT
    std::shared_ptr<Internal::OwnManager> manager;
    QString filename, nameOfProject;
    std::shared_ptr<Internal::OwnProjectFile> file;

public:
    OwnProject(std::shared_ptr<Internal::OwnManager> Manager, const QString & Filename);
};

} // namespace LibprojProjectManager

#endif // LIBPROJPROJECT_H
