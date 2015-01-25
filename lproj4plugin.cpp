#include "lproj4plugin.h"
#include "lproj4constants.h"

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
#include <QJsonDocument>
#include <QJsonObject>

#include <QtPlugin>

using namespace LPROJ4::Internal;

LPROJ4Plugin::LPROJ4Plugin() { }

LPROJ4Plugin::~LPROJ4Plugin() { }

bool LPROJ4Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("Open project"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    //cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("LPROJ4"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return true;
}

void LPROJ4Plugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag LPROJ4Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void LPROJ4Plugin::triggerAction()
{
    pluginFilename = QFileDialog::getOpenFileName(0, QLatin1String("Open File"));
    QFile pluginFile(pluginFilename);
    pluginFile.open( QFile::ReadOnly );
    Q_ASSERT(pluginFile.isOpen());

    // I found that method _fromJson_ is more friendly than similar method _fromRawData_
    readJson(      QJsonDocument::fromJson(pluginFile.readAll()).object()     );
}

//functions that works with Json format
void LPROJ4Plugin::readJson(const QJsonObject & json)
{
    /* TODO
     *
     * there I should implement feature that
     * make sure about correctness of input file.
     */


    qDebug() << json[QLatin1String("author")].toString();
    qDebug() << json[QLatin1String("os_relevant")].toBool();
    qDebug() << json[QLatin1String("message")].toString();
}
