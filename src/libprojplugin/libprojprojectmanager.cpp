#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include <memory>

using std::make_shared;
using std::shared_ptr;
namespace LibprojProjectManager {
namespace Internal {

OwnManager::OwnManager()
{
            qDebug() << "[dbg]\t\tCalling c-tor for OwnManager";
}

/*memory*/
shared_ptr<OwnManager> OwnManager::get_shared_ptr()
{
    qDebug() << "[dbg]\t\tGetting shared_ptr from this of OwnManager instance";
    return shared_from_this();
}
/*memory*/

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
    qDebug() << "[dbg]\t\t\t"+Filename;
    return /*nullptr*/ new OwnProject(get_shared_ptr(), Filename);
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
