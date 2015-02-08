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
    isRw = false;

    return true;
}

void LibprojPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag LibprojPlugin::aboutToShutdown()
{
    delete projectFilename;
    return SynchronousShutdown;
}

void LibprojPlugin::triggerAction()
{
    if (erroneousState = parseMetadata(readProjectFile()))
    {
        qDebug() << "[ok]\tOpening file:" + *projectFilename;
        if (project = ProjectExplorer::ProjectExplorerPlugin::openProject(*projectFilename, &er))
            qDebug() << "[debug]\tProject created";
        /*ProjectExplorer::SessionManager::addProject(project); are we need this ?*/
    }
    else
        qDebug() << "[EE]\tError with opening project file.";
    return;
}

QString LibprojPlugin::readProjectFile()
{
    projectFilename = new QString (QFileDialog::getOpenFileName(0, QLatin1String("Open File")));
    QFile projectFile(*projectFilename);
    if(projectFile.open( QIODevice::ReadWrite | QIODevice::Text )) {
        qDebug() << "[ok]\tFile successfully opened";
        QTextStream inStream(&projectFile);
        if (inStream.status() != QTextStream::Ok) {
            qDebug() << "[EE]\tSomething wrong with QTextStream in f-on readProjectFile()";
            return QString();
        }
        qDebug() << "[ok]\tReading file:";
        return inStream.readAll();
    }
    else
        return QString();
}

/* functions that works with Json format */
bool LibprojPlugin::parseMetadata(const QString & strJson)
{
    if (strJson.isNull() || strJson.isEmpty()) {
        qDebug() << "[EE]\tNothing to parse.";
        return false;
    }
    else {
        bool ok = false;
        parsedMetadata = QtJson::parse(strJson, ok).toMap();
        Q_ASSERT(ok);

        qDebug() << "author:" << parsedMetadata["author"].toString();
        qDebug() << "os_relevant:" << parsedMetadata["os_relevant"].toString();
        qDebug() << "message:" << parsedMetadata["message"].toString();

        qDebug() << "files:" << parsedMetadata["files"].toList();
        if ( parsedMetadata["files"].toList().empty() ) {
            qDebug() << "[EE]\tThere are no files in project (e.g. - corrupted project file)";
            return false;
        }
        else {
            for (const auto& x : parsedMetadata["files"].toList()) {
                filesOfProject.push_back(x.toString());
            }
        }
        return true;
    }
}
