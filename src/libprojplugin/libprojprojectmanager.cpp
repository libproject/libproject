#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include "libproject.h"
#include "libproject_error.h"

typedef ProjectExplorer::Project AbstractProject;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using LibprojManager::Interface::FileSetFactory;
using namespace LibprojManager::Interface::Error;
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
    Q_UNUSED(ErrorString);
    FileSetLoader * loader = FileSetFactory::createFileSet(Filename.toStdString());
    try {
        bool open_result = false;
        open_result = loader->open();
        if (open_result == false)
            throw FileSetRuntimeError(FileSetRuntimeError::UnknownError, "Unsuccessful opening operation");
        return new LibprojManager::Internal::Project(this, loader);
    } catch (const FileSetRuntimeError & re) {
        if (re.getErrorType() == FileSetRuntimeError::AlreadyLoaded)
            return nullptr;
        else
            throw;
    }
}

AbstractProject * Manager::openSubproject(const QString &Filename, QString *ErrorString)
{
    AbstractProject * subproject = this->openProject(Filename, ErrorString);
    this->registerSubproject(subproject);
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

void Manager::registerSubproject(AbstractProject * Subproject)
{
    subprojects.append(Subproject);
}

void Manager::unregisterProject(AbstractProject * /*Project*/)
{
    qDebug() << "Unregistering project";
    project = nullptr;
}

} // namespace Internal
} // namespace LibprojManager
