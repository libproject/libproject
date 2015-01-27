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

#include "../tools/qt-json/json.h"

using namespace Libproj::Internal;

LibprojPlugin::LibprojPlugin() { }

LibprojPlugin::~LibprojPlugin() { }

bool LibprojPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("Open project"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    //cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("Libproj"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

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
    QString pluginFilename = QFileDialog::getOpenFileName(0, QLatin1String("Open File"));
    QFile pluginFile(pluginFilename);
    pluginFile.open( QIODevice::ReadOnly | QIODevice::Text );
    Q_ASSERT(pluginFile.isOpen());

    QTextStream inStream(&pluginFile);
    readJson( inStream.readAll() );
}

//functions that works with Json format
void LibprojPlugin::readJson(const QString & strJson)
{
    /* TODO
     *
     * there I should implement feature that
     * make sure about correctness of input file.
     */

    bool ok = false;
    projectMetadata = QtJson::parse(strJson, ok).toMap();
    Q_ASSERT(ok);

    qDebug() << "author:" << projectMetadata["author"].toString();
    qDebug() << "os_relevant:" << projectMetadata["os_relevant"].toString();
    qDebug() << "message:" << projectMetadata["message"].toString();



}
