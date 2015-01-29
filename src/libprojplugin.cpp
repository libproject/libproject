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


    //setting up itself in QtC environment
    QAction *action = new QAction(tr("Open project"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    //cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));
    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    //========================
    /* PART OF CURRENT TASK */ /* Maxim Kot: Accesible in plugins list at "Build Systems" group */
    menu->menu()->setTitle(tr("Build Systems"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    //some local settings
    isRw = false;
    defFilesList     << QString("main.cpp")
                     << QString("Test.h")
                     << QString("Test.cpp")
                     << QString("Foo.h")
                     << QString("Foo.cpp");


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
    pluginFile.open( QIODevice::ReadWrite | QIODevice::Text );
    if(pluginFile.isOpen())
        qDebug() << "[ok]\tFile successfully opened";
    QTextStream inStream(&pluginFile);
        qDebug() << "[ok]\tReading file:";
    readJson( inStream.readAll() );


    //========================
    /* PART OF CURRENT TASK */ /* Maxim Kot: Can open any file and reports
     * success on opening if file exists and ready for read and write IO.
     *
     * FIXME - insufficient task/ :] What program should do if file exist? Create new instead or append new data?
     */
    isRw = (QMessageBox::Yes == QMessageBox::question(
        0, QString("Open Mode?"),
        QString("Do you want open file in R/W-mode?")));
    qDebug() << "isRw: " << static_cast<unsigned int>(isRw);

    QString pathToDirWithFiles = QFileDialog::getExistingDirectory(0, QLatin1String("Set Dectory where files will are"));
    qDebug() << pathToDirWithFiles;

    //========================
    /* PART OF CURRENT TASK */ /* Maxim Kot: Shows hardcoded list of c++ source files,
     * which are creating automatically on plugin initialization.
     * List of hardcoded files: main.cpp Test.h Test.cpp Foo.h Foo.cpp */
    for (
         QStringList::const_iterator i = defFilesList.cbegin();
         i != defFilesList.cend();
         ++i) {
        /* XXX - there should be smart pointers */
        defFiles.push_back(new QFile(pathToDirWithFiles + QString("/") + (*i)));
        qDebug() << QString("[ok]\tOpening file:\t") + pathToDirWithFiles + QString("/") + (*i);
        defFiles.last()->open(QIODevice::ReadWrite);
    }
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
