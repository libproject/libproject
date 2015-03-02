#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include "libprojprojectfactory.h"

typedef ProjectExplorer::Project AbstractProject;
using std::array;
using std::string;
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
    if (QFileInfo(Filename).suffix() != "libproject")
    {
        qWarning() << "Incorrect file (extension)!";
        return nullptr;
    }

    QFile projectFile(Filename);
    if (projectFile.open( QIODevice::ReadWrite | QIODevice::Text ))
    {
        qDebug() << "File successfully opened";
        QTextStream input(&projectFile);
        if (input.status() != QTextStream::Ok)
        {
            qWarning() << "Something wrong with QTextStream in f-on readProjectFile()";
            return nullptr;
        }
        qDebug() << "About to read file";
        QString qstrContentOfProjectFile = input.readAll();
        qDebug() << qstrContentOfProjectFile;
        if(qstrContentOfProjectFile.isEmpty())
        {
            qWarning() << "File is empty!";
            return nullptr;
        }
        const int numberOfPaths = 2;
        array<QString, numberOfPaths> pathsOfProjectFile = {
            QFileInfo(projectFile).absoluteFilePath(),
            QFileInfo(projectFile).absolutePath() };

        //return new Project(this, qstrContentOfProjectFile.toStdString(), pathsOfProjectFile);

        return
                LibprojManager::
                Interface::
                ProjectFactory::createProject(this, qstrContentOfProjectFile.toStdString(), pathsOfProjectFile);

    }
    else
    {
        qWarning() << "File opened unsuccesfully";
        return nullptr;
    }
}

void Manager::registerProject(AbstractProject * Project)
{
    /* TODO
     * project will be projectS there must be function which appends projects to array*/
    qDebug() << "Registering project";
    project = Project;

    /*Also project must have been registered in plugin's variable*/
    Libproj::Internal::Plugin::setProject(Project);
}

void Manager::unregisterProject(AbstractProject * /*Project*/)
{
    qDebug() << "Unregistering project";
    project = nullptr;
}

} // namespace Internal
} // namespace LibprojManager
