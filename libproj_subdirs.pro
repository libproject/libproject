CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src/libprojplugin/libproj.pro \
    src/libproject \
    test/libprojecttest

documentation.commands = ( \
sed -e \'s%INPUT = %INPUT = $$PWD/src/libproject/%g\' $$PWD/doc/Doxyfile > $$OUT_PWD/Doxyfile ; \
mkdir $$OUT_PWD/doc ; \
cd $$OUT_PWD/doc ; \
doxygen $$OUT_PWD/Doxyfile)
QMAKE_EXTRA_TARGETS += documentation
