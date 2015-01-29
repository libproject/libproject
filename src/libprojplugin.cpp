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

    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));
    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);

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

    parseMetadata( inStream.readAll() );
    openFiles(defFilesList, &erroneousState);
    //if (erroneousState) {
        /* TODO
         * Extremely need to find out how terminate extension */
    //}

    return;
}

//functions that works with Json format
void LibprojPlugin::parseMetadata(const QString & strJson)
{
    /* TODO
     *
     * there I should implement feature that
     * make sure about correctness of input file. */

    bool ok = false;
    projectMetadata = QtJson::parse(strJson, ok).toMap();
    Q_ASSERT(ok);

    qDebug() << "author:" << projectMetadata["author"].toString();
    qDebug() << "os_relevant:" << projectMetadata["os_relevant"].toString();
    qDebug() << "message:" << projectMetadata["message"].toString();



}

void LibprojPlugin::openFiles(const QStringList &filenames, bool *ok)
{
    unsigned int answer = QMessageBox::question(
                             0, QString("Open Mode?"),
                             QString("Do you want open files in R/W-mode?"));
    switch (answer)
    {
    case static_cast<unsigned int>(QMessageBox::Yes) :
        isRw = true;
        *ok = true;
        break;
    case static_cast<unsigned int>(QMessageBox::No) :
        isRw = false;
        *ok = true;
        break;
    default:
        qDebug() << "[EE]\tUnexpected answer from QMessageBox at selecting mode phase";
        *ok = false;
        return;
    }
    qDebug() << "[ok]\tisRw: " << static_cast<unsigned int>(isRw);


    QString pathToDirWithFiles = QFileDialog::getExistingDirectory(0, QLatin1String("Set Dectory where files will are"));
    if ( pathToDirWithFiles.isNull() || pathToDirWithFiles.isEmpty() )
    {
        qDebug() << "[EE]\tUnexpected empty path to dir where should be files of project";
        *ok = false;
        return;
    }
    qDebug() << QString("[ok]\t") + pathToDirWithFiles;

//    TODO
//    if (defFiles.empty())
//    {
//        qDebug() << "[EE]\tThere are no files";
//        *ok = false;
//        return;
//    }

    for (
         QStringList::const_iterator i = filenames.cbegin();
         i != filenames.cend();
         ++i) {
        /* XXX
         * there should be smart pointers */
        defFiles.push_back(new QFile(pathToDirWithFiles + QString("/") + (*i)));
        qDebug() << QString("[ok]\tOpening file:\t") + pathToDirWithFiles + QString("/") + (*i);
        defFiles.last()->open(QIODevice::ReadWrite);
    }
}
