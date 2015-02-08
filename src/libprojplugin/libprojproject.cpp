#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojprojectfile.h"
#include "libprojprojectnodes.h"

using std::shared_ptr;
namespace LibprojProjectManager {
namespace Internal {

OwnProject::OwnProject(std::shared_ptr<Internal::OwnManager> Manager, const QString & Filename)
    : manager(Manager), filename(Filename)
{
    nameOfProject =
            QFileInfo(filename).completeBaseName();
    file =
            shared_ptr<OwnProjectFile> (new OwnProjectFile(shared_ptr<OwnProject>(this), filename));
    rootNode =
            shared_ptr<OwnProjectNode> (new OwnProjectNode(shared_ptr<OwnProject>(this), file) );
}

} // namespace Internal
} // namespace LibprojProjectManager
