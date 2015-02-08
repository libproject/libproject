#include "libprojprojectfile.h"
#include "libprojproject.h"


namespace LibprojProjectManager{
namespace Internal {

OwnProjectFile::OwnProjectFile(std::shared_ptr<OwnProject> Parent, const QString & Filename)
    : Core::IDocument(Parent.get()), project(Parent)
{
    qDebug() << "[dbg]\t\tCalling c-tor for OwnProjectFile";
}

virtual bool OwnProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::save()";
    return false;
}
virtual QString OwnProjectFile::defaultPath() const
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::defaultPath()";
    return QString();
}
virtual QString OwnProjectFile::suggestedFileName() const
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::suggestedFileName()";
    return QString();
}
virtual bool OwnProjectFile::isModified() const
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::isModified()";
    return false;
}
virtual bool OwnProjectFile::isSaveAsAllowed() const
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::isSaveAsAllowed()";
    return false;
}
virtual bool OwnProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    qDebug() << "[dbg]\t\tCalling dummy OwnProjectFile::reload()";
    return false;
}

} // namespace Internal
} // namespace LibprojProjectManager



