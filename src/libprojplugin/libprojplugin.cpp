#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/idocument.h>
#include <projectexplorer/projectexplorer.h>
#include "libprojprojectmanager.h"
#include "libprojplugin.h"
#include "libprojconstants.h"
#include "libprojprojectnodes.h"
#include "libproj_global.h"
#include "libprojproject.h"
#include "json11.hpp"
#include <coreplugin/mimedatabase.h>
#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/iprojectmanager.h>

using ProjectExplorer::FileType;
using ProjectExplorer::FileNode;
using namespace Libproj::Internal;
using json11::Json;
using std::string;
using LibprojProjectManager::Internal::OwnProject;

LibprojProjectManager::Internal::OwnProject * LibprojPlugin::project = nullptr;

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

LibprojPlugin::LibprojPlugin() {
}

LibprojPlugin::~LibprojPlugin() {
}

bool LibprojPlugin::initialize(const QStringList &Arguments, QString *ErrorString)
{
    Q_UNUSED(Arguments)

    /* Registering own mime-type */
    const QLatin1String mimeTypes(":libprojplugin/libprojplugin.mimetypes.xml");
    if (!Core::MimeDatabase::addMimeTypes(mimeTypes, ErrorString))
    {
        qWarning() << ("Error with registering MIME-type");
        return false;
    }

    /* Adding OwnManager to managers pool */
    LibprojProjectManager::Internal::OwnManager * manager = new LibprojProjectManager::Internal::OwnManager();
    IPlugin::addAutoReleasedObject(manager);

    /* Setting up itself in QtC environment */
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

    return true;
}

void LibprojPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag LibprojPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void LibprojPlugin::setProject(LibprojProjectManager::Internal::OwnProject *ProjectToSet)
{
    if(project)
        qWarning() << "Plugin already have associated project";
    project = ProjectToSet;
}

void LibprojPlugin::triggerOpenProjectAction()
{
    QString fileName = QFileDialog::getOpenFileName(0, QString("Open File"));
    if (!(project = qobject_cast<OwnProject*>(ProjectExplorer::ProjectExplorerPlugin::openProject(fileName, &errorString))))
            qWarning() << "OwnManager can not open project";
}

void LibprojPlugin::triggerAddNewFileAction()
{
   switch(QMessageBox::question(nullptr, tr("File exists?"),tr("Are you want to add existing file (Yes) or create new (No)?"),
                                QMessageBox::Yes, QMessageBox::No))
   {
   case QMessageBox::StandardButton::Yes:
   {
       /* TODO  */
       break;
   }
   case QMessageBox::StandardButton::No:
   {
       QString newFilename (QFileDialog::getSaveFileName(nullptr, tr("Where you want to save new file?"), project->document()->filePath().toFileInfo().dir().path()));
       QFile newFile (newFilename);
       if (!newFile.exists())
           newFile.open(QIODevice::ReadWrite | QIODevice::Text);
       else
           qWarning() << "File already exists!";
           /* TODO  I need more secure way than just showing debugging message*/
       project->addFiles(QStringList() << QFileInfo(newFile).absoluteFilePath() );
       break;
   }
   default:
   {
       qWarning() << "Something wrong with answer of message box in triggerAddNewFileAction()";
       break;
   }
   }
}


/* this function isn't needed for now */
/*void LibprojPlugin::saveProjectData(const string & WhatToAppend, const string & WhereToAppend)
{
    qDebug() << QString(WhatToAppend.c_str());
    string
            dumpedData = project->getProjectData().dump(),
            errors;
    bool result = appendToJsonInTextRepresentation<string>(WhatToAppend, WhereToAppend.c_str(), dumpedData);
    if (result) {
       project->setProjectData(Json::parse(dumpedData, errors));
    }
    else
        qWarning() << "Error when modifying json representation";
    qDebug() << QString(errors.c_str());

    QFile projectFile(projectFilename);
    if(!projectFile.open( QIODevice::ReadWrite | QIODevice::Text ))
        qDebug() << "File unsuccessfully opened";
    else
        qDebug() << "File successfully opened";
    QTextStream outStream(&projectFile);
    outStream << dumpedData.c_str();
    projectFile.close();
} */
