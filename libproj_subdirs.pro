CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src/libprojplugin/libproj.pro \
    src/fileset \
    test/filesettest \

crdoc.commands = ( \
#(PWDMOD=\$\(echo $$PWD | sed -e \'s/\//\\\\\//g\'\) ; \
#APPENDX=\\\/src\\\/fileset ; \
#sed -e \"s/INPUT = /INPUT = $PWDMOD$APPENDX/g\" $$PWD/doc/Doxyfile > $$OUT_PWD/Doxyfile ; \
sed -e \'s%INPUT = %INPUT = $$PWD/src/fileset/%g\' $$PWD/doc/Doxyfile > $$OUT_PWD/Doxyfile ; \
mkdir $$OUT_PWD/doc ; \
cd $$OUT_PWD/doc ; \
doxygen $$OUT_PWD/Doxyfile)
QMAKE_EXTRA_TARGETS += crdoc

#sed -i 's/jj/JJ/g' x.x
#sed -e 's/\//\/\\\/g'
