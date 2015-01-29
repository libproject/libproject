#ifndef LPROJ4_H
#define LPROJ4_H

#include "libproj_global.h"
#include <QJsonObject>
#include <extensionsystem/iplugin.h>

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
    virtual void parseMetadata(const QString & strJson);
    virtual void openFiles(const QStringList& filenames, bool * ok);
private slots:
    virtual void triggerAction(); //people says about virtual slots that is bad idea

    //members
private:
    bool isRw; //1 - rw, 0 - ro
    bool erroneousState;
    QVector<QFile *> defFiles;
    QVariantMap projectMetadata;
    QStringList defFilesList;

};

}
}

#endif // LPROJ4_H
