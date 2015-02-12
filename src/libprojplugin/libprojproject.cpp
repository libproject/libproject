#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojprojectfile.h"
#include "libprojprojectnodes.h"
#include <coreplugin/documentmanager.h>
#include "libprojplugin.h"
#include <array>

using Core::IDocument;
using ProjectExplorer::FileNode;
using Libproj::Internal::LibprojPlugin;
using json11::Json;
using std::string;
using std::array;

namespace {
QVariantMap jsonToQVariantMap(const Json& json)
{
    QVariantMap data = QVariantMap();
    const int size = 2;
    array<string, size> toCheck = { /*0: */"author",  /*1: */ "files"  };
    int i = 0;
    /*first key*/
    if (!json[toCheck[0]].string_value().empty())
        data.insert(QString(toCheck[0].c_str()), QVariant(json[toCheck[0]].string_value().c_str()));
    else
        return QVariantMap();
/*END_ first key*/
/*second key*/
    if (!json[toCheck[1]].array_items().empty())
    {
        QStringList files;
        int cnt = 0;
        while (cnt < json[toCheck[1]].array_items().size() )
           files << QString(json[toCheck[1]].array_items().at(cnt++).string_value().c_str());
        data.insert(QString(toCheck[1].c_str()), files);
    }
    else
        return QVariantMap();
 /*END_ second key*/
    return data;
}
}

namespace LibprojProjectManager {
namespace Internal {

OwnProject::OwnProject(OwnManager * Manager, const QString & Filename)
    : manager(Manager),
      filename(Filename)
{
    qDebug() << "Calling c-tor for OwnProject";
    QFileInfo fileInfo(Filename);

    nameOfProject =
            fileInfo.completeBaseName();
        file =
            new OwnProjectFile (this, filename);
    rootNode =
            new OwnProjectNode (this, file);


    rootNode->addFileNodes(jsonToQVariantMap(LibprojPlugin::projectData), fileInfo);
    Core::DocumentManager::addDocument(file, false); //or true? or even - are we need this?

    manager->registerProject(this);
}

QString OwnProject::displayName() const
{
    qDebug() << "Calling OwnProject::displayName()";
    return nameOfProject;
}

IDocument * OwnProject::document() const
{
    qDebug() << "Calling OwnProject::document()";
    return file;
}

ProjectExplorer::IProjectManager * OwnProject::projectManager() const
{
    qDebug() << "Calling OwnProject::projectManager()";
    return manager;
}

ProjectExplorer::ProjectNode * OwnProject::rootProjectNode() const
{
    qDebug() << "Calling OwnProject::rootProjectNode()";
    return rootNode;
}

QStringList OwnProject::files(FilesMode fileMode = AllFiles) const{
    /* TODO
     * must return list of absolute paths*/
    qDebug() << "Calling OwnProject::files(FilesMode)";
    return QStringList() << QFileInfo(filename).absoluteFilePath();
}

} // namespace Internal
} // namespace LibprojProjectManager
