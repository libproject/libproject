#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"

namespace LibprojManager {
namespace Internal {

Manager::Manager()
{
            qDebug() << "Calling c-tor for Manager";
}

QString Manager::mimeType() const
{
            qDebug() << "Comparing with own mime type";
            return QString(Constants::LIBPROJPROJECTMIMETYPE);
}

ProjectExplorer::Project * Manager::openProject(const QString &Filename, QString *ErrorString)
{
    qDebug() << "Opening project";
    if(!QFileInfo(Filename).isFile()) {
            qWarning() << "Project file isn\'t a file" ;
            return nullptr;
    }

    QString contentOfProjectFile = readProjectFile(Filename);
    if (contentOfProjectFile.isEmpty() || contentOfProjectFile.isNull())
        return nullptr;
    else return new Project(this, Filename, contentOfProjectFile);
}

void Manager::registerProject(Project * Project)
{
    /* TODO
     * project will be projectS there must be function which appends projects to array*/
    qDebug() << "Registering project";
    project = Project;

    /*Also project must have been registered in plugin's variable*/
    Libproj::Internal::Plugin::setProject(Project);
}

void Manager::unregisterProject(Project * /*Project*/)
{
    qDebug() << "Unregistering project";
    project = nullptr;
}

QString Manager::readProjectFile(const QString &Filename) const
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
} // namespace LibprojManager
