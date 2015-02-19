#include "libprojplugin.h"
#include "libprojconstants.h"
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include "json11.hpp"
#include <projectexplorer/projectexplorer.h>
#include "libprojprojectmanager.h"
#include "coreplugin/idocument.h"
#include "libprojprojectnodes.h"
#include "libproj_global.h"
#include "libprojproject.h"

using ProjectExplorer::FileType;
using ProjectExplorer::FileNode;
using namespace Libproj::Internal;
using json11::Json;
using std::string;
using LibprojProjectManager::Internal::OwnProject;

Json LibprojPlugin::projectData = Json();
QVector<QFile *> LibprojPlugin::files = QVector<QFile *>();

namespace {
    template <typename T>
    bool appendToJsonInTextRepresentation(const T& valToAppend, const char * whereToAppend, std::string& str)
    {
        int positionOfFilesKey = str.find(whereToAppend);

        int positionOfClosingBracket = str.find (string("]"), positionOfFilesKey);

        switch (str.at(positionOfClosingBracket - 1))
        {
            case ' ':
            {
                str.insert(positionOfClosingBracket - 1, (string(", \"") + valToAppend).append("\""));
            }
            break;
            case '\"':
            {
                str.insert(positionOfClosingBracket, (string(", \"") + valToAppend).append("\""));
            }
            break;
            default:
            {
                return false;
            }
            break;
        }
        return true;
    }
}

LibprojPlugin::LibprojPlugin() { }

LibprojPlugin::~LibprojPlugin() { }

bool LibprojPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    /* setting up itself in QtC environment */
    QAction
            * openProjectAction = new QAction(tr("Open project..."), this),
            * addNewFileAction = new QAction(tr("Add New File..."), this);

    Core::Command
            * openProjectCommand = Core::ActionManager::registerAction(openProjectAction, Constants::OPEN_ACTION_ID, Core::Context(Core::Constants::C_GLOBAL)),
            * addNewFileCommand = Core::ActionManager::registerAction(addNewFileAction, Constants::ADDNEW_ACTION_ID, Core::Context(Core::Constants::C_GLOBAL));

    /*TODO
     * addNewFileCommand must be inactive before opening project*/

    connect(openProjectAction, SIGNAL(triggered()), SLOT(triggerOpenProjectAction()));
    connect(addNewFileAction, SIGNAL(triggered()), SLOT(triggerAddNewFileAction()));

    Core::ActionContainer
            * libprojMenu = Core::ActionManager::createMenu(Constants::MENU_ID);
    libprojMenu->menu()->setTitle(tr("Libproj System"));
    libprojMenu->addAction(openProjectCommand);
    libprojMenu->addAction(addNewFileCommand);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(libprojMenu);

    /* initializing project manager system */
    LibprojProjectManager::Internal::OwnManager
            * manager = new LibprojProjectManager::Internal::OwnManager();
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

void LibprojPlugin::triggerOpenProjectAction()
{
    qDebug() << "Triggering openProjectAction";
    if (erroneousState = parseMetadata(readProjectFile()))
    {
        if (project = qobject_cast<OwnProject*> (ProjectExplorer::ProjectExplorerPlugin::openProject(projectFilename, &er)))
            qDebug() << "Project opened";
        else
            qWarning() << "OwnManager can not open project";
    }
    else
    {
        qWarning() << "Error with opening project file";
    }
    return;
}

QString LibprojPlugin::readProjectFile()
{
    projectFilename = QFileDialog::getOpenFileName(0, QLatin1String("Open File"));
    QFile projectFile(projectFilename);
    if(projectFile.open( QIODevice::ReadWrite | QIODevice::Text ))
    {
        qDebug() << "File successfully opened";
        QTextStream inStream(&projectFile);
        if (inStream.status() != QTextStream::Ok)
        {
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
        qWarning() << "Nothing to parse";
        return false;
    }
    else {
        std::string err;
        qDebug() << "About to start parsing";
        projectData = Json::parse(strJson.toStdString(), err);
        if (!err.empty())
            qWarning() << "Something wrong with parsing:\n" + QString(err.c_str());
        return err.empty() ? true : false;
    }
}

void LibprojPlugin::triggerAddNewFileAction()
{
   switch(QMessageBox::question(nullptr, tr("File exists?"),tr("Are you want to add existing file (Yes) or create new (No)?"),
                                QMessageBox::Yes, QMessageBox::No))
   {
   case QMessageBox::StandardButton::Yes:
   {
       /* TODO
        * ...
        */
       break;
   }
   case QMessageBox::StandardButton::No:
   {
       QString newFilename (QFileDialog::getSaveFileName(nullptr, tr("Where you want to save new file?"), project->document()->filePath().toFileInfo().dir().path()));
       QFile newFile (newFilename);
       if (!newFile.exists())
           newFile.open(QIODevice::ReadWrite | QIODevice::Text);
       else {
           qWarning() << "File already exists!";
           /* TODO
            *  i need more secure way than just showing debugging message*/
       }
       project->addFiles(QStringList() << QFileInfo(newFile).absoluteFilePath() );

       // saveProjectData( QFileInfo(newFile).fileName().toStdString() , string("files") );
       break;
   }
   default:
   {
       qWarning() << "Something wrong with answer of message box in triggerAddNewFileAction()";
       break;
   }
   }
}

void LibprojPlugin::saveProjectData(const string & WhatToAppend, const string & WhereToAppend)
{
    qDebug() << QString(WhatToAppend.c_str());
    string
            dumpedData = projectData.dump(),
            errors;
    bool result = appendToJsonInTextRepresentation<string>(WhatToAppend, WhereToAppend.c_str(), dumpedData);
    if (result)
        projectData = Json::parse(dumpedData, errors);
    else
        qWarning() << "Error when modifying json representation";
    qDebug() << QString(errors.c_str());

    /*writing to file*/
    QFile projectFile(projectFilename);
    if(!projectFile.open( QIODevice::ReadWrite | QIODevice::Text ))
        qDebug() << "File unsuccessfully opened";
    else
        qDebug() << "File successfully opened";
    QTextStream outStream(&projectFile);
    outStream << dumpedData.c_str();
    projectFile.close();
}
