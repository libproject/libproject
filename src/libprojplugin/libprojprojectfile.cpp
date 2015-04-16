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

/*dummies*/
bool ProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    qDebug() << "Calling dummy ProjectFile::save()";
    return false;
}
QString ProjectFile::defaultPath() const
{
    qDebug() << "Calling dummy ProjectFile::defaultPath()";
    return QString();
}
QString ProjectFile::suggestedFileName() const
{
    qDebug() << "Calling dummy ProjectFile::suggestedFileName()";
    return QString();
}
bool ProjectFile::isModified() const
{
    qDebug() << "Calling dummy ProjectFile::isModified()";
    return false;
}
bool ProjectFile::isSaveAsAllowed() const
{
    qDebug() << "Calling dummy ProjectFile::isSaveAsAllowed()";
    return false;
}
bool ProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    qDebug() << "Calling dummy ProjectFile::reload()";
    Q_UNUSED(errorString)
    Q_UNUSED(flag)
    Q_UNUSED(type)
    return true;
}
/*dummies*/

} // namespace Internal
} // namespace LibprojManager



