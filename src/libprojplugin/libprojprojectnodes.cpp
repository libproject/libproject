#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>
#include "libprojprojectmanager.h"
#include "libproject.h"
#include "libproject_error.h"
#include <libgen.h>
#include <cstring>
#include <algorithm>
#include <iterator>

typedef ProjectExplorer::Project AbstractProject;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::Plugin;
using ProjectExplorer::ProjectAction;
using LibprojManager::Internal::Project;
using LibprojManager::Interface::FileSetLoader;
using LibprojManager::Interface::Error::FileSetRuntimeError;
using std::vector;
using std::string;
using std::strstr;
using std::set_intersection;
using std::back_insert_iterator;


namespace  LibprojManager {
namespace Internal {

ProjectNode::ProjectNode(AbstractProject * Project, ProjectFile * ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath().toString()),
      project(Project),
      projectFile(ProjectFile)
{
    qDebug() << "Calling c-tor for ProjectNode";
    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());
}

QList<ProjectExplorer::ProjectAction> ProjectNode::supportedActions(Node * node) const
{
    Q_UNUSED(node);
    return QList<ProjectAction>()
        << ProjectExplorer::ProjectAction::AddNewFile
        << ProjectExplorer::ProjectAction::AddSubProject
        << ProjectExplorer::ProjectAction::RemoveSubProject;
}

bool ProjectNode::canAddSubProject(const QString &proFilePath) const
{
    if (QFileInfo(proFilePath).suffix() == QString("libproject"))
        return true;
    return false;
}
bool ProjectNode::addSubProjects(const QStringList &proFilePaths)
{
    class subprojectIsntInParentFolder {
    };
    try {

        //adding subprojects only to ProjectExplorer tree
        bool result = qobject_cast<Project*>(project)->addFiles(proFilePaths);
        if (result == false)
            return result;


        FileSetLoader * loader = qobject_cast<Project*>(project)->getLoader();
        vector<string> subprojectsPaths;
        for (const QString& filePath : proFilePaths)
        {
            char * dirc, * dname, * dircOfRoot, * dnameOfRoot;

            //getting dirnames of subprojects
            dirc = strdup(filePath.toLocal8Bit());
            dname = dirname(dirc);

            //checking for subproject's membership to folder of parent project
            dircOfRoot = strdup(loader->getPathToNode().c_str());
            dnameOfRoot = dirname(dircOfRoot);
            char * found;
            found = strstr(dircOfRoot, dname);
            if (found == nullptr) //if there is now membership - we dropping exception
                throw subprojectIsntInParentFolder();

            //adding path to suproject to vector of correct candidates
            subprojectsPaths.push_back(filePath.toStdString());
        }

        //adding subprojects to API cache
        loader->addSubprojects(subprojectsPaths);

        //writing changes
        loader->save();

        return true;
    } catch (const FileSetRuntimeError& re) {
        qWarning() << re.what();
        return false;
    } catch (subprojectIsntInParentFolder& ) {
        qWarning() << "This subproject isn't in parent folder";
        return false;
    }

}
bool ProjectNode::removeSubProjects(const QStringList &proFilePaths)
{
    FileSetLoader * loader = qobject_cast<Project*>(project)->getLoader();

    vector<string> candidatesToRemove;
    for (const auto& path : proFilePaths)
        candidatesToRemove.push_back(path.toStdString());

    vector<string> presentSubprojectPaths = loader->getSubprojectsPaths();

    vector<string> approvedToRemove;
    back_insert_iterator<vector<string>> it(approvedToRemove);

    //checking for correctness of paths
    set_intersection(candidatesToRemove.cbegin(), candidatesToRemove.cend(),
                     presentSubprojectPaths.cbegin(), presentSubprojectPaths.cend(),
                     it);

    if (candidatesToRemove.size() == approvedToRemove.size())
    {
        QList<ProjectExplorer::ProjectNode *>
                nodesToRemove,
                currentNodes = qobject_cast<Project *>(project)->getSubprojectNodes();
        for (const string & pathToRemove : approvedToRemove) {
            for (ProjectExplorer::ProjectNode * node : currentNodes)
            {
                if (pathToRemove == qobject_cast<Project *>(project)->getPathToNode().toStdString())
                    nodesToRemove.append(node);
            }
        }

        this->removeProjectNodes(nodesToRemove);

        //this is where friend function in Project class performs action
        for (ProjectExplorer::ProjectNode * node : nodesToRemove)
            qobject_cast<Project*>(project)->subprojectNodes.removeOne(node) ;

        //removing subprojects from API
        loader->removeSubprojects(approvedToRemove);

        //writing changes
        loader->save();
        return true;
    }
    return false;
}

bool ProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    Q_UNUSED(notAdded)
    return qobject_cast<Project*>(project)->addFiles(filePaths);
}

const QString& ProjectNode::getProjectPath() const
{
    return qobject_cast<Project *>(project)->getPathToNode();
}

} // namespace Internal
} // namespace LibprojManager
