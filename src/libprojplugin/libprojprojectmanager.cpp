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

    QString contentOfProjectFile = readProjectFile(Filename/*?*/);
    if (contentOfProjectFile.isEmpty() || contentOfProjectFile.isNull())
        return nullptr;
    else return new OwnProject(this, Filename, contentOfProjectFile);
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

QString OwnManager::readProjectFile(const QString &Filename) const
{
    if (QFileInfo(Filename).suffix() != "libproject")
    {
        qWarning() << "Incorrect file (extension)!";
        return QString();
    }

    QFile projectFile(Filename);
    if (projectFile.open( QIODevice::ReadWrite | QIODevice::Text ))
    {
        qDebug() << "File successfully opened";
        QTextStream input(&projectFile);
        if (input.status() != QTextStream::Ok)
        {
            qWarning() << "Something wrong with QTextStream in f-on readProjectFile()";
            return QString();
        }
        qDebug() << "About to read file";
        return input.readAll();
    }
    else {
        qWarning() << "File opened unsuccesfully";
        return QString();
    }
}

} // namespace Internal
} // namespace LibprojProjectManager
