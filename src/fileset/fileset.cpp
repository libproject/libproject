#include "fileset.h"
#include <fstream>
#include <sstream>
#include <list>
#include "json11.hpp"
#include <iostream>

using std::ifstream;
using std::ostringstream;
using std::string;
using std::list;
using json11::Json;

namespace LibprojManager {
namespace Interface {

    class JsonFileSetLoader : public FileSetLoader
    {
        string sContentOfProjectFile,
                    pathToProjectFile;
        Json jContentOfProjectFile;
        bool loaded;

    public:
        JsonFileSetLoader(const string& ProjectFile)
            : pathToProjectFile(ProjectFile), loaded(false) { }

        virtual const std::string open();
        virtual const list<string> getFileNames() const;
        virtual const string getProjectName() const;
        virtual const string getPathToRootNode() const { return loaded ? pathToProjectFile : string(); }

    private:
        const Json check_json_for_errors();

    };

    const string
    JsonFileSetLoader::open()
    {
        if (loaded) {
            return string("Project file already loaded!");
        }
        ifstream i(pathToProjectFile);
        if(!i) {
            return string("Error with input stream!");
        }
        ostringstream o;
        char buf = 0;
        while (i.get(buf))
            o << buf;

        if (i.eof()) {
            sContentOfProjectFile = o.str();
            jContentOfProjectFile = check_json_for_errors();
            if (jContentOfProjectFile["Error"].is_string()) {
                loaded = false;
                return jContentOfProjectFile["Error"].string_value();
            }
            else if (jContentOfProjectFile.is_null()) {
                loaded = false;
                return string("Unknown error gathered from json11 library!");
            }
            else {
                loaded = true;
                return string();
            }
        }
        else {
            loaded = false;
            return string ("Input stream didn't gave EOF marker!");
        }
    }

    const list<string>
    JsonFileSetLoader::getFileNames() const
    {
        if(!loaded)
            return list<string>();
        list<string> listOfFiles;
        for(const auto& item : jContentOfProjectFile["files"].array_items()) {
            listOfFiles.push_back(item.string_value());
        }
        return listOfFiles;
    }

    const string
    JsonFileSetLoader::getProjectName() const
    {
        if(!loaded)
            return string();
        return jContentOfProjectFile["project"].string_value();
    }

    const Json
    JsonFileSetLoader::check_json_for_errors()
    {
        string err = string(); //starting to parse
        const string error_code = {"{\"Error\" : \""};
        Json j = Json::parse(sContentOfProjectFile, err);

        if (j.is_null()) {
            return Json::parse(error_code + "Empty or broken file!\"}", err);
        }
        if (!j["project"].is_string()) {
            return Json::parse(error_code + "Corrupted or absent project key!\"}", err);
        }
        if (!j["files"].is_array()) {
            return Json::parse(error_code + "Corrupted or absent files key!\"}", err);
        }
        if (j["files"].is_array()) {
            if ( j["files"].array_items().at(0).type() != Json::STRING ) {
                return Json::parse(error_code + "Wrong value type of files key!\"}", err);
            }
        }
        return j;
    }

    FileSetLoader *
    FileSetCreator::create(const string &pathToRootNode) const
    {
        return new JsonFileSetLoader(pathToRootNode);
    }

    FileSetLoader *
    FileSetFactory::createFileSet(const string &pathToRootNode_)
    {
        return AbstractFileSetCreatorSingleton::getCreator().create(pathToRootNode_);
    }

} // namespace Interface
} // namespace LibprojManager
