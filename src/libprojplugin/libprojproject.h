#ifndef LIBPROJPROJECT_H
#define LIBPROJPROJECT_H
#include <projectexplorer/project.h>
#include <memory>

namespace LibprojProjectManager {

namespace Internal {
class OwnManager;
class OwnProjectFile;
class OwnProjectNode;

class OwnProject : public ProjectExplorer::Project
{
    Q_OBJECT
    std::shared_ptr<OwnManager> manager;
    QString filename, nameOfProject;
    std::shared_ptr<OwnProjectFile> file;
    std::shared_ptr<OwnProjectNode> rootNode;

public:
    OwnProject(std::shared_ptr<OwnManager> Manager, const QString & Filename);
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECT_H
