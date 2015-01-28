#ifndef LIBPROJTEST_H
#define LIBPROJTEST_H

#include "libprojtest_global.h"

#include <extensionsystem/iplugin.h>
#include <gmock/gmock.h>
#include "../src/libprojplugin.h"
namespace libprojtest {
namespace Internal {

class libprojtestPlugin : public Libproj::Internal::LibprojPlugin
{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "libprojtest.json")

public:
    MOCK_METHOD0(initialize, bool());
//    libprojtestPlugin();
//    ~libprojtestPlugin();

//    bool initialize(const QStringList &arguments, QString *errorString);
//    void extensionsInitialized();
//    ShutdownFlag aboutToShutdown();

private slots:
//    void triggerAction();
};

} // namespace Internal
} // namespace libprojtest

#endif // LIBPROJTEST_H

