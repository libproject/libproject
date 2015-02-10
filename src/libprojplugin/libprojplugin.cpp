#include "libprojplugin.h"
#include "libprojconstants.h"
#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QtPlugin>
#include "../../tools/qt-json/json.h"

#include <projectexplorer/projectexplorer.h>

#include <extensionsystem/pluginmanager.h>

#include <projectexplorer/iprojectmanager.h>
#include <projectexplorer/session.h>
#include <projectexplorer/projectnodes.h>

#include "libprojprojectmanager.h"

using namespace Libproj::Internal;

QVariantMap LibprojPlugin::parsedMetadata = QVariantMap();
QVector<QFile *> LibprojPlugin::files = QVector<QFile *>();

LibprojPlugin::LibprojPlugin() { }

LibprojPlugin::~LibprojPlugin() { }

bool LibprojPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    /* setting up itself in QtC environment */
    QAction *action = new QAction(tr("Open project"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));
    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("Build Systems"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    /* initializing project manager system */
    LibprojProjectManager::Internal::OwnManager * manager = new LibprojProjectManager::Internal::OwnManager();
    IPlugin::addAutoReleasedObject(manager);

    /* some plugin-wide settings */
    isReadOnly = true;

    return true;
}

void LibprojPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag LibprojPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void LibprojPlugin::triggerAction()
{
    if (erroneousState = parseMetadata(readProjectFile()))
    {
        qDebug() << "Opening file:\t" + projectFilename;
        if (project = ProjectExplorer::ProjectExplorerPlugin::openProject(projectFilename, &er))
            qDebug() << "Project created";
        /*ProjectExplorer::SessionManager::addProject(project); are we need this ?*/
    }
    else
        qWarning() << "Error with opening project file.";
    return;
}

QString LibprojPlugin::readProjectFile()
{
    projectFilename = QFileDialog::getOpenFileName(0, QLatin1String("Open File"));
    QFile projectFile(projectFilename);
    if(projectFile.open( QIODevice::ReadWrite | QIODevice::Text )) {
        qDebug() << "File successfully opened";
        QTextStream inStream(&projectFile);
        if (inStream.status() != QTextStream::Ok) {
            qWarning() << "Something wrong with QTextStream in f-on readProjectFile()";
            return QString();
        }
        qDebug() << "Reading file:";
        return inStream.readAll();
    }
    else
        return QString();
}

/* functions that works with Json format */
bool LibprojPlugin::parseMetadata(const QString & strJson)
{
    if (strJson.isNull() || strJson.isEmpty()) {
        qWarning() << "Nothing to parse.";
        return false;
    }
    else {
        bool ok = false;
        parsedMetadata = QtJson::parse(strJson, ok).toMap();
        Q_ASSERT(ok);
        return true;
    }
}
