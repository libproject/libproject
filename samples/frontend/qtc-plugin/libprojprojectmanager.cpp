#include "libprojprojectmanager.h"
#include "libprojconstants.h"
#include <QDebug>
#include <QFileInfo>
#include "libprojproject.h"
#include <libproject/libproject.h>
#include <libproject/libproject_error.h>

typedef ProjectExplorer::Project AbstractProject;
using std::string;
using LibprojManager::Interface::FileSetLoader;
using LibprojManager::Interface::FileSetFactory;
using namespace LibprojManager::Interface::Error;
namespace LibprojManager {
namespace Internal {

Manager::Manager() { qDebug() << "Calling c-tor for Manager"; }

QString Manager::mimeType() const {
  qDebug() << "Comparing with own mime type";
  return QString(Constants::LIBPROJPROJECTMIMETYPE);
}

ProjectExplorer::Project *Manager::openProject(const QString &Filename,
                                               QString *ErrorString) {
  Q_UNUSED(ErrorString);
  FileSetLoader *loader = FileSetFactory::createFileSet(Filename.toStdString());
  try {
    bool open_result = false;
    open_result = loader->open();
    if (open_result == false)
      throw IncorrectLoaderBehaviour(IncorrectLoaderBehaviour::UnknownError,
                                     "Unsuccessful opening operation");
    return new LibprojManager::Internal::Project(this, loader);
  } catch (const FileSetRuntimeError &re) {
    qWarning() << re.what();
    return nullptr;
  }
}

void Manager::registerProject(AbstractProject *Project) {
  qDebug() << "Registering project";
  projects.append(Project);

  /*Also project must have been registered in plugin's variable*/
  Libproj::Internal::Plugin::setProject(Project);
}

void Manager::unregisterProject(AbstractProject *Project) {
  qDebug() << "Unregistering project";
  projects.removeOne(Project);
}

} // namespace Internal
} // namespace LibprojManager
