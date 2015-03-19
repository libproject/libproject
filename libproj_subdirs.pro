CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src/libprojplugin/libproj.pro \
    src/fileset \
    test/filesettest \

documentation.commands = ( \
sed -e \'s%INPUT = %INPUT = $$PWD/src/fileset/%g\' $$PWD/doc/Doxyfile > $$OUT_PWD/Doxyfile ; \
mkdir $$OUT_PWD/doc ; \
cd $$OUT_PWD/doc ; \
doxygen $$OUT_PWD/Doxyfile)
QMAKE_EXTRA_TARGETS += documentation
