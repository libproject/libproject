#include <coreplugin/documentmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <coreplugin/icontext.h>
#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojplugin.h"
#include "libprojprojectfile.h"
#include "libprojconstants.h"

#include "libproject.h"
#include "libproject_error.h"

using Core::IDocument;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;

namespace LibprojManager {
namespace Internal {

Project::Project(Manager *Manager, const FileSetLoader *Loader)
    : manager(Manager) {
  try {
    qDebug() << "Calling c-tor for Project";

    setId(LibprojManager::Constants::LIBPROJPROJECT_ID);
    setProjectContext(Core::Context(LibprojManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QString pathToRootNode =
        QString::fromStdString(Loader->getPathToNode());
    nameOfProject = QString::fromStdString(Loader->getProjectName());
    for (const auto &filename : Loader->getFileNames())
      projectFiles << QString::fromStdString(filename);

    projectFile = new ProjectFile(this, pathToRootNode);
    rootNode = new ProjectNode(this, projectFile);

    QList<FileNode *> listOfFileNodes;
    listOfFileNodes.push_back(
        new FileNode(QString::fromStdString(Loader->getPathToNode()),
                     FileType::ProjectFileType, false)); // pr. file itself
    for (const auto &x : projectFiles)
      listOfFileNodes.push_back(new FileNode(
          QFileInfo(pathToRootNode).absolutePath() + QString("/") + x,
          FileType::ProjectFileType, false));
    rootNode->addFileNodes(listOfFileNodes);
    Core::DocumentManager::addDocument(projectFile, false);
    manager->registerProject(this);
  } catch (const FileSetRuntimeError & re) {
    qWarning() << re.what();
    throw;
  }
}

QString Project::displayName() const
{
    //qDebug() << "Calling Project::displayName()";
    return nameOfProject;
}

IDocument * Project::document() const
{
    //qDebug() << "Calling Project::document()";
    return projectFile;
}

ProjectExplorer::IProjectManager * Project::projectManager() const
{
    //qDebug() << "Calling Project::projectManager()";
    return manager;
}

ProjectExplorer::ProjectNode * Project::rootProjectNode() const
{
    //qDebug() << "Calling Project::rootProjectNode()";
    return rootNode;
}

QStringList Project::files(FilesMode fileMode) const{
    Q_UNUSED(fileMode);
    /* TODO
     * must return list of absolute paths*/
    //qDebug() << "Calling Project::files(FilesMode)";
    return Project::files();
}

QStringList Project::files() const
{
    QStringList files;
    for (const auto& x : rootNode->fileNodes())
        files << x->displayName();
    return files;
}

bool Project::addFiles(const QStringList &filePaths)
{
    qDebug() << "Calling Project::addFiles(const QStringList &filePaths)";
    QList<FileNode *> fileNodes;
    for (const auto& x : filePaths) {
        if (QFileInfo(x).suffix() != QString("h") && QFileInfo(x).suffix() != QString("cpp"))
        {
            qWarning() << "There are unsupported files";
            continue;
        }
        fileNodes.push_back(new FileNode(x,
                                         /*QFileInfo(x).suffix().compare("cpp") == 0 ?
                                             FileType::SourceType : FileType::HeaderType*/FileType::SourceType,
                                         false));
    }
    rootNode->ProjectNode::addFileNodes(fileNodes);
    return true;
}


} // namespace Internal
} // namespace LibprojManager
