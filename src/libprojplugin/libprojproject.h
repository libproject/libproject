#ifndef LIBPROJPROJECT_H
#define LIBPROJPROJECT_H
#include <projectexplorer/project.h>
#include <memory>

namespace LibprojProjectManager {

namespace Internal {
class OwnManager;
class OwnProjectFile;
class OwnProjectNode;

class OwnProject : public ProjectExplorer::Project, public std::enable_shared_from_this<OwnProject>
{
    Q_OBJECT
    std::shared_ptr<OwnManager> manager;
    QString filename, nameOfProject;
    /*std::list<*/std::shared_ptr<OwnProjectFile> /*>*/file/*s*/;
    std::shared_ptr<OwnProjectNode> rootNode;

public:
    OwnProject(std::shared_ptr<OwnManager> Manager, const QString & Filename);

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;
public /*memory*/:
    std::shared_ptr<OwnProject> get_shared_ptr();
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECT_H
