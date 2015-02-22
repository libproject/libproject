#pragma once
#include <extensionsystem/iplugin.h>
#include <QVector>
#include "libprojprojectnodes.h"
#include "json11.hpp"
#include <string>



class QFile;
namespace ProjectExplorer { class Project; }
namespace LibprojProjectManager { namespace Internal { class OwnProject; } }
namespace Libproj {
namespace Internal {

class LibprojPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "libproj.json")

public:
    LibprojPlugin();
    virtual ~LibprojPlugin();
    virtual bool initialize(const QStringList &Arguments, QString *ErrorString);
    virtual void extensionsInitialized();
    virtual ShutdownFlag aboutToShutdown();
    static void setProject(LibprojProjectManager::Internal::OwnProject * ProjectToSet);
private:
   // void saveProjectData(const std::string & WhatToAppend, const std::string & WhereToAppend);
private slots:
    virtual void triggerOpenProjectAction();
    virtual void triggerAddNewFileAction();
private:
    bool isReadOnly;
    QString projectFilename;
    QString  errorString;
    static LibprojProjectManager::Internal::OwnProject * project;
};

} // namespace Internal
} // namespace Libproj
