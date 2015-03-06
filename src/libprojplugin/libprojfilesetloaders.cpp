//#include "libprojfilesetloaders.h"
//#include <fstream>
//#include <sstream>
//#include <list>
//#include "json11.hpp"
//#include <QDebug>

//using std::ifstream;
//using std::ostringstream;
//using std::string;
//using std::list;
//using json11::Json;

//namespace LibprojManager {
//namespace Interface {

//    class JsonFileSetLoader : public FileSetLoader
//    {
//        string sContentOfProjectFile,
//                    pathToProjectFile;
//        Json jContentOfProjectFile;
//        bool loaded;

//    public:
//        JsonFileSetLoader(const string& ProjectFile)
//            : pathToProjectFile(ProjectFile), loaded(false) { }

//        virtual bool open();
//        virtual const list<string> getFileNames() const;
//        virtual const string getProjectName() const;
//        virtual const string getPathToRootNode() const { return loaded ? pathToProjectFile : string(); }

//    };

//    bool
//    JsonFileSetLoader::open()
//    {
//        if (loaded) {
//            qDebug() << "There is input project data in loader";
//            return false;
//        }
//        ifstream i(pathToProjectFile);
//        if(!i) {
//            qDebug() << "Error with input stream (ifsteam)";
//            return false;
//        }
//        ostringstream o;
//        char buf = 0;
//        while (i.get(buf))
//            o << buf;

//        if (i.eof()) {
//            sContentOfProjectFile = o.str();
//            qDebug() << QString::fromStdString(sContentOfProjectFile);

//            string err; //starting to parse
//            jContentOfProjectFile = Json::parse(sContentOfProjectFile, err);
//            if (!err.empty()) {
//                qDebug() << "Parsing error";
//                return false;
//            }
//            return loaded = true;
//        }

//        qDebug() << "Error with input stream (reading)";
//        /*TODO there must be exc-s*/
//        return false;
//    }

//    const list<string>
//    JsonFileSetLoader::getFileNames() const
//    {
//        if (!loaded)
//            return list<string>();
//        if(!jContentOfProjectFile["files"].is_array())
//            return list<string>();
//        list<string> listOfFiles;
//        for(const auto& item : jContentOfProjectFile["files"].array_items()) {
//            listOfFiles.push_back(item.string_value());
//        }

//        return listOfFiles;
//    }

//    const string
//    JsonFileSetLoader::getProjectName() const
//    {
//        if(!loaded)
//            return string();
//        if(!jContentOfProjectFile["project"].is_string())
//            return string();
//        qDebug() << QString::fromStdString(jContentOfProjectFile["project"].string_value());
//        return jContentOfProjectFile["project"].string_value();
//    }

//    FileSetLoader *
//    FileSetCreator::create(const string &pathToRootNode) const
//    {
//        return new JsonFileSetLoader(pathToRootNode);
//    }

//    FileSetLoader *
//    FileSetFactory::createFileSet(const string &pathToRootNode_)
//    {
//        return AbstractFileSetCreatorSingleton::getCreator().create(pathToRootNode_);
//    }
//} // namespace Interface
//} // namespace LibprojManager
