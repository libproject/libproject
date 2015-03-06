CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += test/libprojtest.pro \
    src/libprojplugin/libproj.pro \
    src/fileset

#CONFIG(debug, debug|release) {
#    DESTDIR = build/debug
#} else {
#    DESTDIR = build/release
#}

# libproj.depends = fileset
