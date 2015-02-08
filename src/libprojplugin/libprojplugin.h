#ifndef LPROJ4_H
#define LPROJ4_H

#include "libproj_global.h"
#include <QJsonObject>
#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>
#include "libprojproject.h"

class QFile;
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
    virtual QString readProjectFile();
    virtual bool parseMetadata(const QString & strJson);
private slots:
    virtual void triggerAction(); //people says about virtual slots that is bad idea

    //members
private:
    bool isRw; //1 - rw, 0 - ro
    bool erroneousState;

    static QVariantMap parsedMetadata;
    QStringList filesOfProject;
    QString * projectFilename;
    QString er;
    ProjectExplorer::Project * project;

};

} // namespace Internal
} // namespace Libproj

#endif // LPROJ4_H
