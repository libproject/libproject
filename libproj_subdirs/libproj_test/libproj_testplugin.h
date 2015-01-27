#ifndef LIBPROJ_TEST_H
#define LIBPROJ_TEST_H

#include "libproj_test_global.h"
#include "../src/libprojplugin.h"
#include <extensionsystem/iplugin.h>
#include <gmock/gmock.h>



namespace libproj_test {
namespace Internal {

class libproj_testPlugin : public Libproj::libprojPlugin
{
    //Q_OBJECT
   // Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "libproj_test.json")

public:
    MOCK_METHOD0(initialize, void());


};

} // namespace Internal
} // namespace libproj_test

#endif // LIBPROJ_TEST_H

