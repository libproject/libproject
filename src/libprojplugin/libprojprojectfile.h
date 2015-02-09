#ifndef LIBPROJPROJECTFILE_H
#define LIBPROJPROJECTFILE_H

#include <QObject>
#include <coreplugin/idocument.h>
#include <memory>

namespace LibprojProjectManager{
namespace Internal {

class OwnProject;
class OwnProjectFile : public Core::IDocument
{
    Q_OBJECT

    std::shared_ptr<OwnProject> project;
public:
    OwnProjectFile(std::shared_ptr<OwnProject> Parent, const QString & Filename); //1st
    OwnProjectFile(OwnProject * Parent, const QString & Filename); //2nd

public /*dummies*/:
    virtual bool save(QString *errorString, const QString &fileName, bool autoSave);
    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const;
    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTFILE_H
