#include "libprojprojectfile.h"
#include "libprojproject.h"
#include <QDebug>
#include "libprojconstants.h"


typedef ProjectExplorer::Project AbstractProject;

namespace LibprojManager{
namespace Internal {

ProjectFile::ProjectFile(AbstractProject * Parent, const QString &Filename)
    : Core::IDocument(Parent),
      project(Parent)
{
    qDebug() << "Calling c-tor for ProjectFile";
    setId("Libproj.ProjectFile");
    setMimeType(QLatin1String(Constants::LIBPROJPROJECTMIMETYPE));
    setFilePath(Utils::FileName::fromString(Filename));
}

bool ProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    return false;
}
QString ProjectFile::defaultPath() const
{
    return QString();
}
QString ProjectFile::suggestedFileName() const
{
    return QString();
}
bool ProjectFile::isModified() const
{
    return false;
}
bool ProjectFile::isSaveAsAllowed() const
{
    return false;
}
bool ProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    Q_UNUSED(type)
    return true;
}

} // namespace Internal
} // namespace LibprojManager



