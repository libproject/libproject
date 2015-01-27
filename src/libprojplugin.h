#ifndef LPROJ4_H
#define LPROJ4_H

#include "libproj_global.h"
#include <QJsonObject>

#include <extensionsystem/iplugin.h>

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

    QVariantMap projectMetadata;
};

}
}

#endif // LPROJ4_H
