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
    OwnProjectFile(std::shared_ptr<OwnProject> Parent, const QString & Filename);

public /*dummies*/:
    virtual bool save(QString *errorString, const QString &fileName, bool autoSave) {  return false; }
    virtual QString defaultPath() const { return QString(); }
    virtual QString suggestedFileName() const { return QString(); }
    virtual bool isModified() const { return false; }
    virtual bool isSaveAsAllowed() const { return false; }
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type) { return false; }
};

} // namespace Internal
} // namespace LibprojProjectManager

#endif // LIBPROJPROJECTFILE_H
