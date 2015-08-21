#include "extensionsystem/iplugin.h"
#include "extensionsystem/extensionsystem_global.h"
#include "extensionsystem/pluginmanager.h"
#include "extensionsystem/pluginspec.h"
#include "aggregation/aggregate.h"
#include <QObject>
#include <QStringList>

using namespace ExtensionSystem;


int main()
{
    PluginManager pluginManager;
    QStringList pathToPlugin;
    pathToPlugin.append(QString("../../../src/"));
    PluginManager::setPluginPaths(pathToPlugin);
    PluginManager::loadPlugins();
    QList<PluginSpec *> specs = pluginManager.plugins();
    qDebug() << specs.front()->name();
    return 0;
}
