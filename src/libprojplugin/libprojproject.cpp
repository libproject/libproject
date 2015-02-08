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
            make_shared<OwnProjectFile> (make_shared<OwnProject>(*this), filename);
    rootNode =
            make_shared<OwnProjectNode> (make_shared<OwnProject>(*this), file);

    Core::DocumentManager::addDocument(file.get(), false); //or true? or even - are we need this?

}

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

} // namespace Internal
} // namespace LibprojProjectManager
