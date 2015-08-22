#pragma once
#include <coreplugin/idocument.h>

namespace ProjectExplorer {
class Project;
}
namespace LibprojManager {
namespace Internal {

class ProjectFile : public Core::IDocument {
  Q_OBJECT

  ProjectExplorer::Project *project;

public:
  ProjectFile(ProjectExplorer::Project *Parent, const QString &Filename);

  virtual bool save(QString *errorString, const QString &fileName,
                    bool autoSave);
  virtual QString defaultPath() const;
  virtual QString suggestedFileName() const;
  virtual bool isModified() const;
  virtual bool isSaveAsAllowed() const;
  virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);
};

} // namespace Internal
} // namespace LibprojManager
