#include "libprojprojectfile.h"
#include "libprojproject.h"


namespace LibprojProjectManager{
namespace Internal {

OwnProjectFile::OwnProjectFile(std::shared_ptr<OwnProject> Parent, const QString & Filename)
    : Core::IDocument(Parent.get()), project(Parent)
{

}

} // namespace Internal
} // namespace LibprojProjectManager



