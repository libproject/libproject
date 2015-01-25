#ifndef LPROJ4_H
#define LPROJ4_H

#include "lproj4_global.h"
#include <QJsonObject>

#include <extensionsystem/iplugin.h>

namespace LPROJ4 {
namespace Internal {

class LPROJ4Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LPROJ4.json")

public:
    LPROJ4Plugin();
    ~LPROJ4Plugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void readJson(const QJsonObject& json);
private slots:
    void triggerAction();

    //members
private:
    QString pluginFilename;
};

} // namespace Internal
} // namespace LPROJ4

#endif // LPROJ4_H

