#include "libprojprojectnodes.h"
#include "libprojprojectfile.h"
using std::shared_ptr;
namespace  LibprojProjectManager {
namespace Internal {

OwnProjectNode::OwnProjectNode(shared_ptr<OwnProject> Project, shared_ptr<OwnProjectFile> ProjectFile)
    : ProjectExplorer::ProjectNode(ProjectFile->filePath().toString()), project(Project),
      projectFile(ProjectFile)
{

}

} // namespace Internal
} // namespace LibprojProjectManager
