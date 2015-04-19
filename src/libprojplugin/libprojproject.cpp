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
#include <vector>

using Core::IDocument;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;
typedef ProjectExplorer::Project AbstractProject;
using std::vector;

namespace LibprojManager {
namespace Internal {

Project::Project(Manager *Manager, FileSetLoader *Loader)
    : manager(Manager), loader(Loader) {
  try {
    qDebug() << "Calling c-tor for Project";

    setId(LibprojManager::Constants::LIBPROJPROJECT_ID);
    setProjectContext(Core::Context(LibprojManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QString pathToRootNode =
        QString::fromStdString(loader->getPathToNode());
    nameOfProject = QString::fromStdString(loader->getProjectName());
    for (const auto &filename : loader->getFileNames())
      projectFiles << QString::fromStdString(filename);

    projectFile = new ProjectFile(this, pathToRootNode);
    rootNode = new ProjectNode(this, projectFile);

    QList<FileNode *> listOfFileNodes;
    listOfFileNodes.push_back(
        new FileNode(QString::fromStdString(loader->getPathToNode()),
                     FileType::ProjectFileType, false)); // pr. file itself
    for (const auto &x : projectFiles)
      listOfFileNodes.push_back(new FileNode(
          QFileInfo(pathToRootNode).absolutePath() + QString("/") + x,
          FileType::ProjectFileType, false));
    rootNode->addFileNodes(listOfFileNodes);
    if(loader->countSubprojects() > 0)
    {
        vector<string> paths = loader->getSubprojectsPaths();
        QStringList subprojects, failed;
        for (const auto& path : paths)
        {
            subprojects.append(QFileInfo(pathToRootNode).absolutePath()
                               + QString("/")
                               + QString::fromStdString(path));
        }
        rootNode->addFiles(subprojects, &failed);
    }
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
    qDebug() << "Calling files()";
    QStringList files;
    for (const auto& x : rootNode->fileNodes())
        files << x->displayName();
    return files;
}

bool Project::addFiles(const QStringList &filePaths)
{
    qDebug() << "Calling Project::addFiles(const QStringList &filePaths)";
    QList<FileNode *> fileNodes;
    QList<ProjectExplorer::ProjectNode *> newSubprojectNodes;
    QString * err = new QString();
    for (const auto& path : filePaths) {
        if (QFileInfo(path).suffix() == QString("h") || QFileInfo(path).suffix() == QString("cpp"))
        {
            fileNodes.push_back(new FileNode(path,
                                             QFileInfo(path).suffix() == QString("cpp")?
                                             FileType::SourceType:
                                             FileType::HeaderType,
                                             false)
                                );
        }
        else if (QFileInfo(path).suffix() == QString("libproject"))
        {
            AbstractProject * subproject = manager->openProject(path, err);
            newSubprojectNodes.push_back(subproject->rootProjectNode());
        }

    }
    if(!fileNodes.isEmpty())
        rootNode->ProjectNode::addFileNodes(fileNodes);
    if(!newSubprojectNodes.isEmpty())
    {
        rootNode->ProjectNode::addProjectNodes(newSubprojectNodes);
        subprojectNodes += newSubprojectNodes;
    }
    if(fileNodes.isEmpty() && newSubprojectNodes.isEmpty())
        return false;
    return true;
}

ProjectFile * Project::getProjectFile() const
{
    return projectFile;
}


} // namespace Internal
} // namespace LibprojManager
