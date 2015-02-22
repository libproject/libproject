#include <array>
#include <coreplugin/documentmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <coreplugin/icontext.h>
#include "libprojproject.h"
#include "libprojprojectmanager.h"
#include "libprojplugin.h"
#include "libprojprojectfile.h"
#include "libprojconstants.h"
#include "json11.hpp"

using Core::IDocument;
using ProjectExplorer::FileNode;
using ProjectExplorer::FileType;
using Libproj::Internal::LibprojPlugin;
using json11::Json;
using std::string;
using std::array;

namespace LibprojProjectManager {
namespace Internal {

OwnProject::OwnProject(OwnManager * Manager, const QString & Filename, const QString &ContentOfProjectFile)
    : manager(Manager),
      filename(Filename)
{
    qDebug() << "Calling c-tor for OwnProject";

    setId(LibprojProjectManager::Constants::LIBPROJPROJECT_ID);
    setProjectContext(Core::Context(LibprojProjectManager::Constants::PROJECTCONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    std::string errorString;
    projectData = Json::parse(ContentOfProjectFile.toStdString(), errorString);
    if (!errorString.empty())
    {
        /* TODO */
    }
    else {
        QFileInfo fileInfo(Filename);
        nameOfProject =  fileInfo.completeBaseName();
        file = new OwnProjectFile (this, filename);
        rootNode = new OwnProjectNode (this, file);
        qDebug() << "Starting to add files to root node";
        QList<FileNode*> listOfFileNodes;

        //project file itself:
        listOfFileNodes.push_back(new FileNode(fileInfo.absoluteFilePath(), FileType::ProjectFileType,  false));

        for (const auto& x: jsonToQVariantMap(projectData)["files"].toStringList())
            listOfFileNodes.push_back(new FileNode(fileInfo.absolutePath()+QString("/")+x, FileType::ProjectFileType, false));

        rootNode->addFileNodes(listOfFileNodes);
        Core::DocumentManager::addDocument(file, false);
        manager->registerProject(this);
    }
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

QStringList OwnProject::files(FilesMode fileMode) const{
    /* TODO
     * must return list of absolute paths*/
    qDebug() << "Calling OwnProject::files(FilesMode)";
    return OwnProject::files();
}

QStringList OwnProject::files() const
{
    QStringList files;
    for (const auto& x : rootNode->fileNodes())
        files << x->displayName();
    return files;
}

bool OwnProject::addFiles(const QStringList &filePaths)
{
    qDebug() << "Calling OwnProject::addFiles(const QStringList &filePaths)";
    QList<FileNode *> fileNodes;
    for (const auto& x : filePaths) {
        if (QFileInfo(x).suffix() != QString("h") && QFileInfo(x).suffix() != QString("cpp"))
        {
            qWarning() << "There are unsupported files";
            continue;
        }
        fileNodes.push_back(new FileNode(x,
                                         /*QFileInfo(x).suffix().compare("cpp") == 0 ?
                                             FileType::SourceType : FileType::HeaderType*/FileType::SourceType,
                                         false));
    }
    rootNode->ProjectNode::addFileNodes(fileNodes);
    return true;
}

QVariantMap OwnProject::jsonToQVariantMap(const Json& json) const
{
    /* TODO
     * there must be recursive function for parsing more complex files*/
    QVariantMap data = QVariantMap();
    const int size = 2;
    array<string, size> toCheck = { /*0: */"author",  /*1: */ "files"  };
    /*first key*/
    if (!json[toCheck[0]].string_value().empty())
        data.insert(QString(toCheck[0].c_str()), QVariant(json[toCheck[0]].string_value().c_str()));
    else
        return QVariantMap(); /*TODO: I'm not controlling it*/
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

} // namespace Internal
} // namespace LibprojProjectManager
