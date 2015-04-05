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
#include "json.hpp"
#include "libproject_error.h"
#include <libgen.h>

using std::ifstream;
using std::ostringstream;
using std::string;
using std::list;
using nlohmann::json;
using namespace LibprojManager::Interface::Error;
using std::map;
using std::ofstream;

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
        string sContentOfProjectFile;
        const string pathToProjectFile;
        json jContentOfProjectFile;
        json jChangedContentOfProjectFile;
        map<string, FileSetLoader *> subprojects;
        bool loaded; //! flag which becoming true condition when instance is busy

    public:
        /*!
         * \brief Constructs interface instance
         * \param[in] ProjectFile is std::string and it describes path to .libproject
         * file
         */
        JsonFileSetLoader(const string & ProjectFile)
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
         * \brief save
         */
        virtual void save();

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
        /*!
         * \brief addSubprojects
         * \param subp
         * \return
         */
        virtual void addSubprojects(const std::vector<std::string> & subp);

    private:

        /*!
         * \brief checkProjectFileForErrors
         * \return Json object within the content of project file or Json object
         * within single key "Error" string value of which contains description of error
         */
        const json checkProjectFileForErrors(std::ifstream &) const;

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
            throw FileSetRuntimeError(FileSetRuntimeError::AlreadyLoaded, "Project already loaded");
        ifstream i(pathToProjectFile);
        if(!i)
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, "Error with input stream");

        jContentOfProjectFile = checkProjectFileForErrors(i); /// Checking JSON-data for consistentness, correctness and reading it

        if (jContentOfProjectFile.count("Error") != 0) { /// Checking data for higher abstract errors - project level errors
            loaded = false;
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, jContentOfProjectFile["Error"].get<string>());
        } else {
            if (jContentOfProjectFile["subprojects"].is_array()) /// Checking project data for subprojects
               subprojects = loadSubprojects(); /// If above is true loading subprojects
        }

        i.close();
        return loaded = true;
    }

    void
    JsonFileSetLoader::save()
    {
        if (jChangedContentOfProjectFile != jContentOfProjectFile)
            jContentOfProjectFile = jChangedContentOfProjectFile;

        ofstream o;
        o.open(pathToProjectFile, std::ofstream::out);
        if (!o)
            throw FileSetRuntimeError(FileSetRuntimeError::UnknownError, "Unidentified problem with output file stream");
        o << std::setw(4) << jContentOfProjectFile;
        return;
    }

    const list<string>
    JsonFileSetLoader::getFileNames() const
    {
        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get file names on not loaded interface");
        list<string> listOfFiles;
        for(const auto& item : jContentOfProjectFile["files"]) {
            listOfFiles.push_back(item.get<string>());
        }
        return listOfFiles;
    }

    const string
    JsonFileSetLoader::getProjectName() const
    {
        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get project name on not loaded interface");
        return jContentOfProjectFile["project"].get<string>();
    }

    const int
    JsonFileSetLoader::countSubprojects() const
    {
        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to count subprojects on not loaded interface");
        return subprojects.empty()? 0 : subprojects.size();

    }

    map<string, FileSetLoader *>
    JsonFileSetLoader::getSubprojectLoaders()
    {
        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get subprojects loaders on not loaded interface");
        return subprojects;

    }

    void
    JsonFileSetLoader::addSubprojects(const std::vector<std::string> &subp)
    {
        auto getDirPath = [this](const string& s) -> const string {
            //#ifdef __linux__
            std::size_t found = s.find_last_of("//");
            //#endif
            return s.substr(0, found + 1);
        };

        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to add subprojects on not loaded interface");
        jChangedContentOfProjectFile = jContentOfProjectFile;
        if(jChangedContentOfProjectFile["subprojects"].is_null())
            jChangedContentOfProjectFile["subprojects"] = { };
        for(const auto& sp : subp) {
            const char * const cPathToProjectFile = pathToProjectFile.c_str();
            const char * const dname = dirname((char* const)cPathToProjectFile);
            int whereRelativePathStarts = string(dname).length() + 1; // +1 because we need to skip "/" symbol
            string relativePath = sp.substr(whereRelativePathStarts);
            jChangedContentOfProjectFile["subprojects"].push_back(relativePath);
        }
        // TODO check for already added subprojects

    }

    const json JsonFileSetLoader::checkProjectFileForErrors(ifstream& ifs) const
    {
        const string error_code = {"{\"Error\" : \""};
        try {
            json j;
            j << ifs;
            if (ifs.eof() == false)
                return json::parse(error_code + "Input stream didn't gave EOF marker!\"}");

            if (j.is_null())
                return json::parse(error_code + "Empty or broken file!\"}");

            if (j["project"].is_string() == false)
                return json::parse(error_code + "Corrupted or absent project key!\"}");

            if (j["files"].is_array() == false)
                return json::parse(error_code + "Corrupted or absent files key!\"}");
            else if (j["files"].at(0).is_string() == false)
                return json::parse(error_code + "Wrong values type of files key!\"}");

            if (j["subprojects"].is_array())
                if (j["subprojects"].at(0).is_string() == false)
                    return json::parse(error_code + "Wrong values type of subprojects key!\"}");
            else if (!j["subprojects"].is_null() && !j["subprojects"].is_array())
                return json::parse(error_code + "Corrupted or absent subprojects key!\"}");


            return j;
        } catch (const std::exception& e) {
            throw FileSetRuntimeError(FileSetRuntimeError::UnknownError, (string)"Error gathered from json API: " + (string)e.what());
        }
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
            for (const auto& x: jContentOfProjectFile["subprojects"]) {
                        pathSub = x.get<string>();
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
