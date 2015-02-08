#ifndef LIBPROJPROJECTFILE_H
#define LIBPROJPROJECTFILE_H

#include <QObject>
#include <coreplugin/idocument.h>
#include <memory>

namespace LibprojProjectManager{
class OwnProject;
namespace Internal {

class OwnProjectFile : public Core::IDocument
{
    Q_OBJECT

    std::shared_ptr<OwnProject> project;
public:
    OwnProjectFile(std::shared_ptr<OwnProject> Parent, const QString & Filename);
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTFILE_H
