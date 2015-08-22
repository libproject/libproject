#pragma once
#include <extensionsystem/iplugin.h>
//#include <QVector>
#include "libprojprojectnodes.h"
#include <string>
#include "libprojconstants.h"

class QFile;
namespace ProjectExplorer {
class Project;
}
namespace Libproj {
namespace Internal {

class Plugin : public ExtensionSystem::IPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "proj.json")

public:
  Plugin();
  virtual ~Plugin();
  virtual bool initialize(const QStringList &Arguments, QString *ErrorString);
  virtual void extensionsInitialized();
  virtual ShutdownFlag aboutToShutdown();
  static void setProject(ProjectExplorer::Project *ProjectToSet);
private slots:
  virtual void triggerOpenProjectAction();
  virtual void triggerAddNewFileAction();

private:
  bool isReadOnly;
  QString projectFilename;
  QString errorString;
  static ProjectExplorer::Project *project;
};

} // namespace Internal
} // namespace Libproj
