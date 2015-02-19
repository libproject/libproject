#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"

namespace LibprojProjectManager {
namespace Internal {

OwnManager::OwnManager()
{
            qDebug() << "Calling c-tor for OwnManager";
}

QString OwnManager::mimeType() const
{
            qDebug() << "Comparing with own mime type";
            return QString(Constants::LIBPROJPROJECTMIMETYPE);
}

ProjectExplorer::Project * OwnManager::openProject(const QString &Filename, QString *ErrorString)
{
    qDebug() << "Opening project";
    if(!QFileInfo(Filename).isFile()) {
            qWarning() << "Project file isn\'t a file" ;
            return nullptr;
    }
    return new OwnProject(this, Filename);
}

void OwnManager::registerProject(OwnProject * Project)
{
    /* TODO
     * project will be projectS there must be function which appends projects to array*/
    qDebug() << "Registering project";
    project = Project;
}

void OwnManager::unregisterProject(OwnProject * /*Project*/)
{
    qDebug() << "Unregistering project";
    project = nullptr;
}

} // namespace Internal
} // namespace LibprojProjectManager
