#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include "libprojfilesetloaders.h"

typedef ProjectExplorer::Project AbstractProject;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using LibprojManager::Interface::FileSetFactory;
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

    FileSetLoader * loader = FileSetFactory::createFileSet(Filename.toStdString());
    if (loader->open())

        return new LibprojManager::Internal::Project(this, loader);

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
