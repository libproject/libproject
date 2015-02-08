#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojprojectfile.h"

using std::shared_ptr;
using LibprojProjectManager::Internal::OwnProjectFile;
namespace LibprojProjectManager {

OwnProject::OwnProject(std::shared_ptr<Internal::OwnManager> Manager, const QString & Filename)
    : manager(Manager), filename(Filename)
{
    nameOfProject = QFileInfo(filename).completeBaseName();
    file = shared_ptr<OwnProjectFile> (new OwnProjectFile(this, filename));
}


} // namespace LibprojProjectManager
