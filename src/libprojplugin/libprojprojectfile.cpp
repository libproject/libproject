#include "libprojprojectfile.h"
#include "libprojproject.h"
#include <QDebug>
#include "libprojconstants.h"

namespace LibprojProjectManager{
namespace Internal {

OwnProjectFile::OwnProjectFile(OwnProject * Parent, const QString &Filename)
    : Core::IDocument(Parent),
      project(Parent)
{
    qDebug() << "Calling c-tor for OwnProjectFile";
    setId("Libproj.ProjectFile");
    setMimeType(QLatin1String(Constants::LIBPROJPROJECTMIMETYPE));
    setFilePath(Utils::FileName::fromString(Filename));
}

/*dummies*/
bool OwnProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    qDebug() << "Calling dummy OwnProjectFile::save()";
    return false;
}
QString OwnProjectFile::defaultPath() const
{
    qDebug() << "Calling dummy OwnProjectFile::defaultPath()";
    return QString();
}
QString OwnProjectFile::suggestedFileName() const
{
    qDebug() << "Calling dummy OwnProjectFile::suggestedFileName()";
    return QString();
}
bool OwnProjectFile::isModified() const
{
    qDebug() << "Calling dummy OwnProjectFile::isModified()";
    return false;
}
bool OwnProjectFile::isSaveAsAllowed() const
{
    qDebug() << "Calling dummy OwnProjectFile::isSaveAsAllowed()";
    return false;
}
bool OwnProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    qDebug() << "Calling dummy OwnProjectFile::reload()";
    return false;
}
/*dummies*/

/*QString OwnProjectFile::mimeType() const
{
    qDebug() << "Calling OwnProjectFile::mimeType()";
    return QString (Constants::LIBPROJPROJECTMIMETYPE);
}*/
} // namespace Internal
} // namespace LibprojProjectManager



