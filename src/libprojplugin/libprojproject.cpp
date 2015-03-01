#include <array>
#include <coreplugin/documentmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <coreplugin/icontext.h>
#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojplugin.h"
#include "libprojprojectfile.h"
#include "libprojconstants.h"


using Core::IDocument;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using json11::Json;
using std::string;
using std::array;
typedef array<QString, 2> paths;

namespace LibprojManager {
namespace Internal {

Project::Project(Manager * Manager, const string & ContentOfProjectFile, const paths& Paths)
    : manager(Manager)
{
    qDebug() << "Calling c-tor for Project";

    setId(LibprojManager::Constants::LIBPROJPROJECT_ID);
    setProjectContext(Core::Context(LibprojManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    if (readProjectFile(ContentOfProjectFile))
    {
        enum { WithFile = 0, WithoutFile = 1 };

        nameOfProject = getProjectName();
        projectFiles = getFileNames();
        projectFile = new ProjectFile (this, Paths.at(WithFile));
        rootNode = new ProjectNode (this, projectFile);

       QList<FileNode*> listOfFileNodes;
        //project file itself:
        listOfFileNodes.push_back(new FileNode(Paths.at(WithFile), FileType::ProjectFileType,  false));
        for (const auto& x: projectFiles)
            listOfFileNodes.push_back(new FileNode(Paths.at(WithoutFile)+QString("/")+x, FileType::ProjectFileType, false));
        rootNode->addFileNodes(listOfFileNodes);
        Core::DocumentManager::addDocument(projectFile, false);
        manager->registerProject(this);
    }
    else
    {
        /*drop exc when PM will be extracted*/
    }
}


QString Project::displayName() const
{
    qDebug() << "Calling Project::displayName()";
    return nameOfProject;
}

IDocument * Project::document() const
{
    qDebug() << "Calling Project::document()";
    return projectFile;
}

ProjectExplorer::IProjectManager * Project::projectManager() const
{
    qDebug() << "Calling Project::projectManager()";
    return manager;
}

ProjectExplorer::ProjectNode * Project::rootProjectNode() const
{
    qDebug() << "Calling Project::rootProjectNode()";
    return rootNode;
}

QStringList Project::files(FilesMode fileMode) const{
    /* TODO
     * must return list of absolute paths*/
    qDebug() << "Calling Project::files(FilesMode)";
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

bool Project::readProjectFile(const string & ContentOfProjectFile_)
{

    string err;
    contentOfProjectFile = Json::parse(ContentOfProjectFile_, err);
    if (err.empty())
        return true;
    else
        return false;
}

const QStringList  Project::getFileNames() const
{
    jsonToQVM convert(contentOfProjectFile, std::initializer_list<std::string>({"files"}));
    QVariantMap m;
   return convert(m) ["files"].toStringList();
}

const QString  Project::getProjectName() const
{
    jsonToQVM convert(contentOfProjectFile, std::initializer_list<std::string>({"project"}));
    QVariantMap m;
   return convert(m) ["project"].toString();
}

} // namespace Internal
} // namespace LibprojManager
