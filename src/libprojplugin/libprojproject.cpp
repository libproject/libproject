#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojprojectfile.h"
#include "libprojprojectnodes.h"
#include <coreplugin/documentmanager.h>
#include "libprojplugin.h"

using Core::IDocument;
using ProjectExplorer::FileNode;
using Libproj::Internal::LibprojPlugin;

namespace LibprojProjectManager {
namespace Internal {

OwnProject::OwnProject(OwnManager * Manager, const QString & Filename)
    : manager(Manager),
      filename(Filename)
{
    qDebug() << "Calling c-tor for OwnProject";
    QFileInfo fileInfo(Filename);

    nameOfProject =
            fileInfo.completeBaseName();
        file =
            new OwnProjectFile (this, filename);
    rootNode =
            new OwnProjectNode (this, file);


    rootNode->addFileNodes(LibprojPlugin::parsedMetadata, fileInfo);
    Core::DocumentManager::addDocument(file, false); //or true? or even - are we need this?

    manager->registerProject(this);
}

QString OwnProject::displayName() const
{
    qDebug() << "Calling OwnProject::displayName()";
    return nameOfProject;
}

IDocument * OwnProject::document() const
{
    qDebug() << "Calling OwnProject::document()";
    return file;
}

ProjectExplorer::IProjectManager * OwnProject::projectManager() const
{
    qDebug() << "Calling OwnProject::projectManager()";
    return manager;
}

ProjectExplorer::ProjectNode * OwnProject::rootProjectNode() const
{
    qDebug() << "Calling OwnProject::rootProjectNode()";
    return rootNode;
}

QStringList OwnProject::files(FilesMode fileMode = AllFiles) const{
    /* TODO
     * must return list of absolute paths*/
    qDebug() << "Calling OwnProject::files(FilesMode)";
    return QStringList() << QFileInfo(filename).absoluteFilePath();
}

} // namespace Internal
} // namespace LibprojProjectManager
