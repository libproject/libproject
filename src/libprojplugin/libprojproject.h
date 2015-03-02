#pragma once
#include "libprojplugin.h"
#include <projectexplorer/project.h>
#include "libprojinterface.h"
#include <string>
#include <vector>
#include "json11.hpp"
#include <array>
#include "libprojconstants.h"

namespace LibprojManager {
namespace Internal {

class Manager;
class ProjectFile;
class ProjectNode;

class Project : public ProjectExplorer::Project, protected Interface::FileSetLoader
{
    Q_OBJECT
    Manager * manager;
    QString nameOfProject;
    QStringList projectFiles;
    ProjectFile * projectFile;
    ProjectNode * rootNode;

public:
    Project(Manager * Manager, const std::string & ContentOfProjectFile,
            const std::array<QString, 2> &Paths);

    QString displayName() const;
    Core::IDocument *document() const;
    ProjectExplorer::IProjectManager *projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;
    QStringList files() const;
    bool addFiles(const QStringList &filePaths);

protected /*interface*/:
    json11::Json contentOfProjectFile;

    virtual bool readProjectFile(const std::string & ContentOfProjectFile_);
    virtual const QStringList getFileNames() const;
    virtual const QString getProjectName() const;

    class jsonToQVM {
        typedef std::initializer_list<std::string> ils;
    private:
        json11::Json json;
        std::vector<std::string> keywords;

        QString write(QVariantMap& qvm, json11::Json j, QList<std::string> k)
        {
            for (const auto& keyword : k) {
                if (j[keyword].is_object()) //for LPROJ-12
                    return "Unsupported field in project file\n";
                else if (j[keyword].is_null())
                    qvm.insert(QString(keyword.c_str()),
                    QVariant());
                else if (j[keyword].is_number())
                    qvm.insert(QString(keyword.c_str()),
                    QVariant(j[keyword].number_value()));
                else if (j[keyword].is_bool())
                    qvm.insert(QString(keyword.c_str()),
                    QVariant(j[keyword].bool_value()));
                else if (j[keyword].is_string()) //aggregates fields like author
                    qvm.insert(QString(keyword.c_str()),
                        QVariant(QString(j[keyword].
                        string_value().c_str())));
                else if (j[keyword].is_array() //now aggregates "file"-field
                    && j[keyword].array_items().at(0).is_string()) {
                    QStringList strings;
                    for (const auto& str : j[keyword].array_items())
                        strings << str.string_value().c_str();
                    qvm.insert(QString(keyword.c_str()),
                        QVariant(strings));
                } else return "Unknown error\n";
                return QString();
            }
        }
    public:

        explicit jsonToQVM(const json11::Json& json_, const ils& keywords_)
        : json(json_), keywords(keywords_) { }

        QVariantMap& operator()(QVariantMap& writeTo)
        {
            if (!writeTo.isEmpty()) {
                qWarning() << "Container isn't empty";
                return writeTo;
            } else {
                QString result;
                for (const auto& keyword : keywords)
                    result += write(writeTo, json, QList<std::string>() << keyword);
                if (result.isEmpty())
                    return writeTo;
                else {
                    writeTo.clear();
                    qWarning() << "Error when writing QVariantMap";
                    return writeTo;
                }
            }
        }
    };
};

} // namespace Internal
} // namespace LibprojManager
