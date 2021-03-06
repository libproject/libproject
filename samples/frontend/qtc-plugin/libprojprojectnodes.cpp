#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
#include "libprojproject.h"
#include "libprojplugin.h"
#include <QFile>
#include "libprojprojectmanager.h"
#include "libproject/libproject.h"
#include "libproject/libproject_error.h"
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

namespace LibprojManager {
namespace Internal {

ProjectNode::ProjectNode(AbstractProject *Project, ProjectFile *ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath() /*.toString()*/),
      project(Project), projectFile(ProjectFile) {
  qDebug() << "Calling c-tor for ProjectNode";
  setDisplayName(qobject_cast<LibprojManager::Internal::Project *>(project)
                     ->displayName());
}

QList<ProjectExplorer::ProjectAction>
ProjectNode::supportedActions(Node *node) const {
  Q_UNUSED(node);
  return QList<ProjectAction>()
         << ProjectExplorer::ProjectAction::AddNewFile
         << ProjectExplorer::ProjectAction::AddSubProject
         << ProjectExplorer::ProjectAction::RemoveSubProject;
}

bool ProjectNode::canAddSubProject(const QString &proFilePath) const {
  if (QFileInfo(proFilePath).suffix() == QString("libproject"))
    return true;
  return false;
}
bool ProjectNode::addSubProjects(const QStringList &proFilePaths) {
  class subprojectIsntInParentFolder {};
  try {

    // adding subprojects only to ProjectExplorer tree
    bool result = qobject_cast<Project *>(project)->addFiles(proFilePaths);
    if (result == false)
      return result;

    FileSetLoader *loader = qobject_cast<Project *>(project)->getLoader();
    vector<string> subprojectsPaths;
    for (const QString &filePath : proFilePaths) {
      char *dirc, *dname, *dircOfRoot, *dnameOfRoot;

      // getting dirnames of subprojects
      dirc = strdup(filePath.toLocal8Bit());
      dname = dirname(dirc);

      // checking for subproject's membership to folder of parent project
      dircOfRoot = strdup(loader->getPathToNode().c_str());
      dnameOfRoot = dirname(dircOfRoot);
      char *found;
      found = strstr(dircOfRoot, dname);
      if (found == nullptr) // if there is now membership - we dropping
                            // exception
        throw subprojectIsntInParentFolder();

      // adding path to suproject to vector of correct candidates
      subprojectsPaths.push_back(filePath.toStdString());
    }

    // adding subprojects to API cache
    loader->addSubprojects(subprojectsPaths);

    // writing changes
    loader->save();

    return true;
  } catch (const FileSetRuntimeError &re) {
    qWarning() << re.what();
    return false;
  } catch (subprojectIsntInParentFolder &) {
    qWarning() << "This subproject isn't in parent folder";
    return false;
  }
}
bool ProjectNode::removeSubProjects(const QStringList &proFilePaths) {
  FileSetLoader *loader = qobject_cast<Project *>(project)->getLoader();

  // checking for correctness of paths
  for (const auto &proFilePath : proFilePaths) {
    try {
      string path = loader->findSubprojectByPath(proFilePath.toStdString())
                        ->getPathToNode();
    } catch (const FileSetRuntimeError &re) {
      qWarning() << QString(re.what()) + proFilePath;
      return false;
    }
  }

  QList<ProjectExplorer::ProjectNode *> nodesToRemove,
      currentNodes = qobject_cast<Project *>(project)->getSubprojectNodes();
  for (const QString &proFilePath : proFilePaths) {
    for (ProjectExplorer::ProjectNode *node : currentNodes) {
      if (proFilePath == dynamic_cast<ProjectNode *>(node)->getProjectPath())
        nodesToRemove.append(node);
    }
  }

  this->removeProjectNodes(nodesToRemove);

  // this is where friend function in Project class performs action
  for (ProjectExplorer::ProjectNode *node : nodesToRemove)
    qobject_cast<Project *>(project)->subprojectNodes.removeOne(node);

  // removing subprojects from API
  vector<string> relativePaths;
  string directoryOfParent =
      QFileInfo(getProjectPath()).absolutePath().toStdString();
  for (const QString &path : proFilePaths) {
    string relativePath =
        path.toStdString().substr(directoryOfParent.length() + 1);
    relativePaths.push_back(relativePath);
  }
  loader->removeSubprojects(relativePaths);

  // writing changes
  loader->save();
  return true;
}

bool ProjectNode::addFiles(const QStringList &filePaths,
                           QStringList *notAdded) {
  Q_UNUSED(notAdded)
  return qobject_cast<Project *>(project)->addFiles(filePaths);
}

const QString ProjectNode::getProjectPath() const {
  return qobject_cast<Project *>(project)->getPathToNode();
}

} // namespace Internal
} // namespace LibprojManager
