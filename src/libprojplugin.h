#ifndef LPROJ4_H
#define LPROJ4_H

#include "libproj_global.h"
#include <QJsonObject>
#include <QFile> //WHY I CANT USE FORWARD DECLARATION OF QFILE????---------------------------------------------
#include <extensionsystem/iplugin.h>
#include <memory>

namespace Libproj {
namespace Internal {

class LibprojPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "libproj.json")

public:
    LibprojPlugin();
    virtual ~LibprojPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString);
    virtual void extensionsInitialized();
    virtual ShutdownFlag aboutToShutdown();

private:
    virtual void readJson(const QString & strJson);
private slots:
    virtual void triggerAction(); //people says about virtual slots that is bad idea

    //members
private:
    bool isRw; //1 - rw, 0 - ro
    QVector<QFile *> defFiles;
    QVariantMap projectMetadata;
    QStringList defFilesList;
};

}
}

#endif // LPROJ4_H
