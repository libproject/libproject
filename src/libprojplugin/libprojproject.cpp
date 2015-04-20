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
#include <list>

using Core::IDocument;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;
typedef ProjectExplorer::Project AbstractProject;
using std::vector;
using std::list;

namespace LibprojManager {
namespace Internal {

Project::Project(Manager *Manager, FileSetLoader *Loader)
    : manager(Manager), loader(Loader) {
  try {
    qDebug() << "Calling c-tor for Project";

    setId(LibprojManager::Constants::LIBPROJPROJECT_ID);
    setProjectContext(Core::Context(LibprojManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    //getting path to node
    pathToNode =
        QString::fromStdString(loader->getPathToNode());

    //getting name of project
    nameOfProject = QString::fromStdString(loader->getProjectName());

    //init ProjectFile instance
    projectFile = new ProjectFile(this, pathToNode);

    //init ProjectNode instance
    rootNode = new ProjectNode(this, projectFile);

    //creating project file FileNode instance
    FileNode * projectFileNode = new FileNode(pathToNode, FileType::ProjectFileType, false);
    rootNode->addFileNodes(QList<FileNode *>() << projectFileNode);

    //creating FileNode instances for project files
    list<string> filesToAdd = loader->getFileNames();
    QStringList files, failed;
    for (const auto& file : filesToAdd)
    {
        files.append(QFileInfo(pathToNode).absolutePath()
                     + QString("/")
                     + QString::fromStdString(file));
    }
    rootNode->addFiles(files, &failed);

    //creating subproject instances
    if(loader->countSubprojects() > 0)
    {
        vector<string> paths = loader->getSubprojectsPaths();
        QStringList subprojects;
        for (const auto& path : paths)
        {
            subprojects.append(QFileInfo(pathToNode).absolutePath()
                               + QString("/")
                               + QString::fromStdString(path));
        }
        rootNode->addFiles(subprojects, &failed);
    }

    //i don't clearly understand this stuff yet
    Core::DocumentManager::addDocument(projectFile, false);
    manager->registerProject(this);

  } catch (const FileSetRuntimeError & re) {
    qWarning() << re.what();
    throw;
  }
}

QString Project::displayName() const
{
    return nameOfProject;
}

IDocument * Project::document() const
{
    return projectFile;
}

ProjectExplorer::IProjectManager * Project::projectManager() const
{
    return manager;
}

ProjectExplorer::ProjectNode * Project::rootProjectNode() const
{
    return rootNode;
}

QStringList Project::files(FilesMode fileMode) const{
    Q_UNUSED(fileMode);

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

//this is function which is called on demand to add files by other instances of plugin classes
bool Project::addFiles(const QStringList &filePaths)
{
    qDebug() << "Calling Project::addFiles(const QStringList &filePaths)";
    QList<FileNode *> fileNodes;
    QList<ProjectExplorer::ProjectNode *> newSubprojectNodes;
    QString * err = new QString();

    for (const auto& path : filePaths) {

        //here we preparing to add nodes of source files
        if (QFileInfo(path).suffix() == QString("h") || QFileInfo(path).suffix() == QString("cpp"))
        {
            fileNodes.push_back(new FileNode(path,
                                             QFileInfo(path).suffix() == QString("cpp")?
                                             FileType::SourceType:
                                             FileType::HeaderType,
                                             false)
                                );
        }

        //here we preparing nodes of subprojects only
        //because node of main project has been added in Project c-tor
        else if (QFileInfo(path).suffix() == QString("libproject"))
        {
            AbstractProject * subproject = manager->openProject(path, err);
            newSubprojectNodes.push_back(subproject->rootProjectNode());
        }
    }

    //actually adding nodes performing here
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
