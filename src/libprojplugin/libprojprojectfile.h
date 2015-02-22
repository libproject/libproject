#pragma once
#include <coreplugin/idocument.h>

namespace LibprojManager{
namespace Internal {

class Project;
class ProjectFile : public Core::IDocument
{
    Q_OBJECT

    Project * project;
public:
    ProjectFile(Project * Parent, const QString & Filename);

public /*dummies*/:
    virtual bool save(QString *errorString, const QString &fileName, bool autoSave);
    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const;
    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);

};

} // namespace Internal
} // namespace LibprojManager
