#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include <memory>

using std::make_shared;
namespace LibprojProjectManager {
namespace Internal {

OwnManager::OwnManager()
{
            qDebug() << "[dbg]\t\tCalling c-tor for OwnManager";
}

QString OwnManager::mimeType() const
{
            qDebug() << "[dbg]\t\tComparing with own mime type";
            return QString(Constants::LIBPROJPROJECTMIMETYPE);
}

ProjectExplorer::Project * OwnManager::openProject(const QString &Filename, QString *ErrorString)
{
    qDebug() << "[dbg]\t\tOpening project";
    if(!QFileInfo(Filename).isFile()) {
            qDebug() << " [EE]\tProject file isn\'t a file" ;
            return nullptr;
    }
    return /*nullptr*/ new OwnProject(make_shared<OwnManager>(*this), Filename);
}

void OwnManager::registerProject(std::shared_ptr<OwnProject> Project)
{
    qDebug() << "[dbg]\t\tRegistering project";
    project = Project;
}

void OwnManager::unregisterProject(std::shared_ptr<OwnProject>& /*Project*/)
{
    qDebug() << "[dbg]\t\tUnregistering project";
    project.reset();
}

} // namespace Internal
} // namespace LibprojProjectManager
