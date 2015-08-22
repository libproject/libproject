#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/idocument.h>
#include <projectexplorer/projectexplorer.h>
#include "libprojprojectmanager.h"
#include "libprojplugin.h"
#include "libprojconstants.h"
#include "libprojprojectnodes.h"
#include "libproj_global.h"
#include "libprojproject.h"
#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/iprojectmanager.h>
#include <utils/mimetypes/mimedatabase.h>

typedef ProjectExplorer::Project AbstractProject;
using ProjectExplorer::FileType;
using ProjectExplorer::FileNode;
using namespace Libproj::Internal;
using std::string;
using LibprojManager::Internal::Project;
AbstractProject *Plugin::project = nullptr;

Plugin::Plugin() {}

Plugin::~Plugin() {}

bool Plugin::initialize(const QStringList &Arguments, QString *ErrorString) {
  Q_UNUSED(Arguments)
  Q_UNUSED(ErrorString)

  /* Registering own mime-type */
  const QString mimeTypes(":libprojplugin/libprojplugin.mimetypes.xml");
  Utils::MimeDatabase::addMimeTypes(mimeTypes);

  /* Adding Manager to managers pool */
  LibprojManager::Internal::Manager *manager =
      new LibprojManager::Internal::Manager();
  IPlugin::addAutoReleasedObject(manager);

  /* Setting up itself in QtC environment */
  QAction *openProjectAction = new QAction(tr("Open project..."), this),
          *addNewFileAction = new QAction(tr("Add New File..."), this);

  Core::Command *openProjectCommand = Core::ActionManager::registerAction(
                    openProjectAction, Constants::OPEN_ACTION_ID,
                    Core::Context(Core::Constants::C_GLOBAL)),
                *addNewFileCommand = Core::ActionManager::registerAction(
                    addNewFileAction, Constants::ADDNEW_ACTION_ID,
                    Core::Context(Core::Constants::C_GLOBAL));

  /*TODO
   * addNewFileCommand must be inactive before opening project*/

  connect(openProjectAction, SIGNAL(triggered()),
          SLOT(triggerOpenProjectAction()));
  connect(addNewFileAction, SIGNAL(triggered()),
          SLOT(triggerAddNewFileAction()));

  Core::ActionContainer *libprojMenu =
      Core::ActionManager::createMenu(Constants::MENU_ID);
  libprojMenu->menu()->setTitle(tr("Libproj System"));
  libprojMenu->addAction(openProjectCommand);
  libprojMenu->addAction(addNewFileCommand);
  Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)
      ->addMenu(libprojMenu);

  return true;
}

void Plugin::extensionsInitialized() {}

ExtensionSystem::IPlugin::ShutdownFlag Plugin::aboutToShutdown() {
  return SynchronousShutdown;
}

void Plugin::setProject(AbstractProject *ProjectToSet) {
  if (project)
    qWarning() << "Plugin already have associated project";
  project = ProjectToSet;
}

void Plugin::triggerOpenProjectAction() {
  QString fileName = QFileDialog::getOpenFileName(0, QString("Open File"));
  if (!(project = qobject_cast<Project *>(
            ProjectExplorer::ProjectExplorerPlugin::openProject(fileName,
                                                                &errorString))))
    qWarning() << "Manager can not open project";
}

void Plugin::triggerAddNewFileAction() {
  switch (QMessageBox::question(
      nullptr, tr("File exists?"),
      tr("Are you want to add existing file (Yes) or create new (No)?"),
      QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::StandardButton::Yes: {
    /* TODO  */
    break;
  }
  case QMessageBox::StandardButton::No: {
    QString newFilename(QFileDialog::getSaveFileName(
        nullptr, tr("Where you want to save new file?"),
        project->document()->filePath().toString()));
    QFile newFile(newFilename);
    if (!newFile.exists())
      newFile.open(QIODevice::ReadWrite | QIODevice::Text);
    else
      qWarning() << "File already exists!";
    /* TODO  I need more secure way than just showing debugging message*/
    qobject_cast<Project *>(project)
        ->addFiles(QStringList() << QFileInfo(newFile).absoluteFilePath());
    break;
  }
  default: {
    qWarning() << "Something wrong with answer of message box in "
                  "triggerAddNewFileAction()";
    break;
  }
  }
}
