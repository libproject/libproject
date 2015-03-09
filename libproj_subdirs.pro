CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src/libprojplugin/libproj.pro \
    src/fileset \
    test/filesettest \
    test/libprojtest
