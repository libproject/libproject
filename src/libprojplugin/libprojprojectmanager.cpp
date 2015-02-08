#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"

namespace LibprojProjectManager {
namespace Internal {

OwnManager::OwnManager()
{
            qDebug() << "[dbg]\t\tStarting OwnManager instance";
}

QString OwnManager::mimeType() const
{
            qDebug() << "[dbg]\t\tComparing with own mime type";
            return QString(Constants::LIBPROJPROJECTMIMETYPE);
}

ProjectExplorer::Project * OwnManager::openProject(const QString &Filename, QString *ErrorString)
{
    if(!QFileInfo(Filename).isFile()) {
            qDebug() << " [EE]\tProject file isn\'t a file" ;
            return nullptr;
    }
    return nullptr /* new OwnProject(this, fileName)*/;
}

} // namespace Internal
} // namespace LibprojProjectManager
