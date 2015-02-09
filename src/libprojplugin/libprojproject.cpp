#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojprojectfile.h"
#include "libprojprojectnodes.h"
#include <memory>
#include <coreplugin/documentmanager.h>

using std::make_shared;
using Core::IDocument;
namespace LibprojProjectManager {
namespace Internal {

OwnProject::OwnProject(std::shared_ptr<Internal::OwnManager> Manager, const QString & Filename)
    : manager(Manager), filename(Filename)
{
    qDebug() << "[dbg]\t\tCalling c-tor for OwnProject";
    nameOfProject =
            QFileInfo(filename).completeBaseName();
    file =
            make_shared<OwnProjectFile> (this, filename);
    rootNode =
            make_shared<OwnProjectNode> (this, file);

    Core::DocumentManager::addDocument(file.get(), false); //or true? or even - are we need this?

}

/*memory*/
std::shared_ptr<OwnProject> OwnProject::get_shared_ptr()
{
    qDebug() << "[dbg]\t\tGetting shared_ptr from this of OwnProject instance";
    return shared_from_this();
}
/*memory*/

QString OwnProject::displayName() const
{
    qDebug() << "[dbg]\t\tCalling OwnProject::displayName()";
    return nameOfProject;
}

IDocument * OwnProject::document() const
{
    qDebug() << "[dbg]\t\tCalling OwnProject::document()";
    return file.get();
}

ProjectExplorer::IProjectManager * OwnProject::projectManager() const
{
    qDebug() << "[dbg]\t\tCalling OwnProject::projectManager()";
    return manager.get();
}

ProjectExplorer::ProjectNode * OwnProject::rootProjectNode() const
{
    qDebug() << "[dbg]\t\tCalling OwnProject::rootProjectNode()";
    return rootNode.get();
}

QStringList OwnProject::files(FilesMode fileMode) const{
    /*must return list of absolute paths*/
    qDebug() << "[dbg]\t\tCalling OwnProject::files(FilesMode)";
    return QStringList() << QFileInfo(filename).absoluteFilePath();
}

} // namespace Internal
} // namespace LibprojProjectManager
