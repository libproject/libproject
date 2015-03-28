/*!
* \file
* \brief The source file which contains realization of input interface
*
* Here is located JSON-like realization of abstract input interface
*/

#include "libproject.h"
#include <fstream>
#include <sstream>
#include <list>
#include "json11.hpp"
#include "libproject_error.h"

using std::ifstream;
using std::ostringstream;
using std::string;
using std::list;
using json11::Json;
using namespace LibprojManager::Interface::Error;
using std::map;

/*!
 * \brief Covers all classes of present project except Qt creator plugin
 * instance
 */
namespace LibprojManager {

/*!
 * \brief In this namespace placed all interface-related classes
 */
namespace Interface {

     /*!
     * \brief Gives to user Json-realization of abstract interface
     *
     * It leans on json11 library which gives functional for interaction with
     * JSON syntax
     */
    class JsonFileSetLoader : public FileSetLoader
    {
        string sContentOfProjectFile,
                    pathToProjectFile;
        Json jContentOfProjectFile;
        map<string, FileSetLoader*> subprojects;
        bool loaded; //! flag which becoming true condition when instance is busy

    public:
        /*!
         * \brief Constructs interface instance
         * \param[in] ProjectFile is std::string and it describes path to .libproject
         * file
         */
        JsonFileSetLoader(const string& ProjectFile)
            : pathToProjectFile(ProjectFile), loaded(false) { }

        /*!
         * \brief Reads file and parsing it
         * \return boolean true if file opened successfully
         */
        virtual bool open();

        /*!
         * \brief Gives to user list<string> of filenames
         * \return list<string> of filenames of project or drops exception if project wasn't loaded
         */
        virtual const list<string> getFileNames() const;

        /*!
         * \brief Gives to user project name
         * \return project name in string format or drops exception if project wasn't loaded
         */
        virtual const string getProjectName() const;

        /*!
         * \brief Gives to user path to node
         * \return path to node in string format or drops exception if project wasn't loaded
         */
        virtual const string getPathToNode() const {
            return loaded ? pathToProjectFile :
                        throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded,
                                                  "Trying to get path to root node on not loaded interface"); }

        /*!
          * \brief Gives number of subprojects to user
          * \return number of subprojects. Zero implied
          */
        virtual const int countSubprojects() const;

        /*!
         * \brief Gives STL map with pointers to loaders
         * \return map<string, FileSetLoader*> of subprojects associated with its names
         */
        virtual map<string, FileSetLoader *> getSubprojectLoaders();

    private:

        /*!
         * \brief checkProjectFileForErrors
         * \return Json object within the content of project file or Json object
         * within single key "Error" string value of which contains description of error
         */
        const Json checkProjectFileForErrors(const std::string &sContent);

        /*!
         * \brief loadSubprojects loads nested subprojects
         * \return map<string, FileSetLoader*> of interface instances
         */
        map<std::string, FileSetLoader *> loadSubprojects();

    };

    bool
    JsonFileSetLoader::open()
    {
        if (loaded)
            throw FileSetRuntimeError(FileSetRuntimeError::AlreadyLoaded, "Project alredy loaded");
        ifstream i(pathToProjectFile);
        if(!i)
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, "Error with input stream");
        ostringstream o;
        char buf = 0;
        while (i.get(buf))
            o << buf;
        if (!i.eof()) {
            loaded = false;
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, "Input stream didn't gave EOF marker");
        }

        sContentOfProjectFile = o.str(); /// Getting string-data which gathered from .libproject file

        jContentOfProjectFile = checkProjectFileForErrors(sContentOfProjectFile); /// Checking JSON-data for consistentness, correctness and reading it

        if (jContentOfProjectFile["Error"].is_string()) { /// Checking data for higher abstract errors - project level errors
            loaded = false;
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, jContentOfProjectFile["Error"].string_value());
        } else if (jContentOfProjectFile.is_null()) {
            loaded = false;
            throw FileSetRuntimeError(FileSetRuntimeError::UnknownError, "Unknown error gathered from json11 library");
        } else {
            if (jContentOfProjectFile["subprojects"].is_array()) /// Checking project data for subprojects
               subprojects = loadSubprojects(); /// If above is true loading subprojects
        }
        return loaded = true;
    }

    const list<string>
    JsonFileSetLoader::getFileNames() const
    {
        if(!loaded)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get file names on not loaded interface");
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
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get project name on not loaded interface");
        return jContentOfProjectFile["project"].string_value();
    }

    const int
    JsonFileSetLoader::countSubprojects() const
    {
        if(!loaded)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to count subprojects on not loaded interface");
        return subprojects.size();

    }

    map<string, FileSetLoader *>
    JsonFileSetLoader::getSubprojectLoaders()
    {
        if(loaded == false) { }
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get subprojects loaders on not loaded interface");
        return subprojects;

    }

    const Json
    JsonFileSetLoader::checkProjectFileForErrors(const string& sContent)
    {
        string err = string(); //starting to parse
        const string error_code = {"{\"Error\" : \""};
        Json j = Json::parse(sContentOfProjectFile, err);

        if (j.is_null())
            return Json::parse(error_code + "Empty or broken file!\"}", err);

        if (!j["project"].is_string())
            return Json::parse(error_code + "Corrupted or absent project key!\"}", err);

        if (!j["files"].is_array())
            return Json::parse(error_code + "Corrupted or absent files key!\"}", err);
        else if ( j["files"].array_items().at(0).type() != Json::STRING)
            return Json::parse(error_code + "Wrong values type of files key!\"}", err);

        if (j["subprojects"].is_array())
            if (j["subprojects"].array_items().at(0).type() != Json::STRING)
                return Json::parse(error_code + "Wrong values type of subprojects key!\"}", err);
        else if (!j["subprojects"].is_null() && !j["subprojects"].is_array())
            return Json::parse(error_code + "Corrupted or absent subprojects key!\"}", err);
        return j;
    }

    map<string, FileSetLoader*>
    JsonFileSetLoader::loadSubprojects()
    {
        auto getDirPath = [this](const string& s) -> const string {
            //#ifdef __linux__
            std::size_t found = s.find_last_of("//");
            //#endif
            return s.substr(0, found + 1);
        };
        try {
            map<string, FileSetLoader*> subprojectLoaders;
            string pathHead, pathSub, nameOfSubproject;
            FileSetLoader * loaderOfSubproject;
            pathHead = getDirPath(this->pathToProjectFile);
            for (const auto& x: jContentOfProjectFile["subprojects"].array_items()) {
                        pathSub = x.string_value();
                        loaderOfSubproject = FileSetFactory::createFileSet(pathHead + pathSub);
                        loaderOfSubproject->open();
                        nameOfSubproject = loaderOfSubproject->getProjectName();
                        subprojectLoaders.emplace(nameOfSubproject, loaderOfSubproject);
            }

            return subprojectLoaders;
            } catch (const FileSetRuntimeError& re) {
                throw FileSetRuntimeError(FileSetRuntimeError::BrokenSubproject, string(re.what()));
            }
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
