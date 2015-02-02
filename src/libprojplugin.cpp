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

    /* some local settings */
    isRw = false;

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

    /* through if-condition assigning result of parsing. Where parsing itself launching from
     * argument that is return value (QString) of function readProjectFile() */
    if (erroneousState = parseMetadata(readProjectFile()))
        openFiles(filesOfProject, &erroneousState);
    else
        qDebug() << "[EE]\tAborting extension";
    /* TODO
     * I need to explain how to terminate extension. */

    return;
}

QString LibprojPlugin::readProjectFile()
{
    QString pluginFilename = QFileDialog::getOpenFileName(0, QLatin1String("Open File"));
    QFile pluginFile(pluginFilename);
    if(pluginFile.open( QIODevice::ReadWrite | QIODevice::Text )) {
        qDebug() << "[ok]\tFile successfully opened";
        QTextStream inStream(&pluginFile);
        if (inStream.status() != QTextStream::Ok) {
            qDebug() << "[EE]\tSomething wrong with QTextStream in f-on readProjectFile()";
            return QString();
        }
        qDebug() << "[ok]\tReading file:";
        return inStream.readAll();
    }
    else
        /* for error checking needs */
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

void LibprojPlugin::openFiles(const QStringList &filenames, bool *ok)
{
    QMessageBox::StandardButton answer = QMessageBox::question(
                             0, QString("Open Mode?"),
                             QString("Do you want open files in R/W-mode?"));
    switch (answer)
    {
    case QMessageBox::Yes :
        isRw = true;
        *ok = true;
        break;
    case QMessageBox::No :
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

    if (filesOfProject.isEmpty()) {
        qDebug() << "[EE]\Filenames are not loaded into plugin system.";
        *ok = false;
        return;
    }
    else {
        for (
             QStringList::const_iterator i = filesOfProject.cbegin();
             i != filesOfProject.cend();
             ++i) {
            /* XXX
             * there should be smart pointers */
            instancesOfFiles.push_back(new QFile(pathToDirWithFiles + QString("/") + (*i)));
            qDebug() << QString("[ok]\tOpening file:\t") + pathToDirWithFiles + QString("/") + (*i);
            instancesOfFiles.last()->open(QIODevice::ReadWrite);
        }
    }
}
