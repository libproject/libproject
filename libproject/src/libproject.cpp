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
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>

using std::ifstream;
using std::ostringstream;
using std::string;
using std::list;
using nlohmann::json;
using namespace LibprojManager::Interface::Error;
using std::map;
using std::ofstream;
using std::vector;
using std::set;

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
        /*virtual*/ bool open();

        /*!
         * \brief Gives to user list<string> of filenames
         * \return list<string> of filenames of project or drops exception if project wasn't loaded
         */
        /*virtual*/ const list<string> getFileNames() const;

        /*!
         * \brief Saves changes of .libproject file to it
         */
        /*virtual*/ void save();

        /*!
         * \brief Gives to user project name
         * \return project name in string format or drops exception if project wasn't loaded
         */
        /*virtual*/ const string getProjectName() const;

        /*!
         * \brief Gives to user path to node
         * \return path to node in string format or drops exception if project wasn't loaded
         */
        /*virtual*/ const string getPathToNode() const {
            return loaded ? pathToProjectFile :
                        throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded,
                                                  "Trying to get path to root node on not loaded interface"); }

        /*!
          * \brief Gives std::vector<std::string> of path to subprojects to user
          * in relative path format (to project)
          * \return vector of subprojects paths. Zero-sized vector implied
          */
        /*virtual*/ const vector<string> getSubprojectsPaths() const;

        /*!
          * \brief Gives number of subprojects to user
          * \return number of subprojects. Zero implied
          */
        /*virtual*/ int countSubprojects() const;

        /*!
         * \brief Gives STL map with pointers to loaders
         * \return map<string, FileSetLoader*> of subprojects associated with its names
         */
        /*virtual*/ map<string, FileSetLoader *> getSubprojectLoaders();

        /*!
         * \brief This function performs adding existing subprojects which are present on
         * filesystem to cache of .libproject file. But NOT saves it.
         * \param[in] std::vector of pathes to subprojects
         */
        /*virtual*/ void addSubprojects(const vector<string> & subp);

        /*!
         * \brief This function performs adding existing subproject which is present on
         * filesystem to cache of .libproject file. But NOT saves it.
         * \param[in] std::string with path to subproject
         */
        /*virtual*/ void addSubproject(const string & subp);

        /*!
         * \brief This function performs removing existing subprojects in cache or in saved
         * .libproject file
         * \param[in] std::vector of pathes to subprojects
         */
        /*virtual*/ void removeSubprojects(const vector<string> & subp);

        /*!
         * \brief This function performs removing existing subproject in cache or in saved
         * .libproject file
         * \param[in] std::string with path to subproject
         */
        /*virtual*/ void removeSubproject(const string& s);

        /*!
         * \brief This function performs searching existing subproject loader by path to
         * subproject
         * \param[in] std::string with path to subproject
         * \return loader of subproject
         */
        /*virtual*/ FileSetLoader * findSubprojectByPath(const std::string & path) const;

    private:

        /*!
         * \brief checkProjectFileForErrors
         * \return Json object within the content of project file or Json object
         * within single key "Error" string value of which contains description of error
         */
        const json checkProjectFileForErrors(ifstream &) const;

        /*!
         * \brief loadSubprojects loads nested subprojects
         */
        void loadSubprojects();

    };

    bool
    JsonFileSetLoader::open()
    {
        if (loaded)
            throw FileSetRuntimeError(FileSetRuntimeError::AlreadyLoaded, "Project already loaded");
        ifstream i(pathToProjectFile);
        if(!i)
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, "Error with input stream");

        jChangedContentOfProjectFile = jContentOfProjectFile = checkProjectFileForErrors(i); /// Checking JSON-data for consistentness, correctness and reading it

        if (jContentOfProjectFile.count("Error") != 0) { /// Checking data for higher abstract errors - project level errors
            loaded = false;
            throw FileSetRuntimeError(FileSetRuntimeError::IncorrectSource, jContentOfProjectFile["Error"].get<string>());
        } else {
            if (jContentOfProjectFile["subprojects"].is_array()) /// Checking project data for subprojects
               loadSubprojects(); /// If above is true loading subprojects
        }

        i.close();
        return loaded = true;
    }

    void
    JsonFileSetLoader::save()
    {
        if (jChangedContentOfProjectFile == jContentOfProjectFile)
            return;

        jContentOfProjectFile = jChangedContentOfProjectFile;
        ofstream o(pathToProjectFile);
        if (!o)
            throw FileSetRuntimeError(FileSetRuntimeError::UnknownError, "Unidentified problem with output file stream");
        o << std::setw(4) << jContentOfProjectFile;
        o.close();
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

    const vector<string>
    JsonFileSetLoader::getSubprojectsPaths() const
    {
        if (loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to get subprojects paths on not loaded interface");
        auto& subprojects = jChangedContentOfProjectFile["subprojects"];
        vector<string> paths;
        for (const auto& path : subprojects)
        {
            paths.push_back(path);
        }
        return paths;
    }

    int JsonFileSetLoader::countSubprojects() const
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
        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to add subprojects on not loaded interface");

        char * cPathToProjectFile, * dname;
        cPathToProjectFile = strdup(pathToProjectFile.c_str());
        dname = dirname(cPathToProjectFile);
        int whereRelativePathStarts = string(dname).length() + 1; // +1 because we need to skip "/" symbol
        set<string> candidates;
        for(const auto& sp : subp) {
            string relativePath = sp.substr(whereRelativePathStarts);

            ifstream checkSubprojectsStream;
            checkSubprojectsStream.open(string(dname)+string("/")+relativePath);
            json check = checkProjectFileForErrors(checkSubprojectsStream);
            if (check.count("Error") != 0)
                throw FileSetRuntimeError(FileSetRuntimeError::BrokenSubproject, "Trying to add broken subproject(s)");
            checkSubprojectsStream.close();

            if(jChangedContentOfProjectFile.count("subprojects") != 0) {
                for (const auto& cachedSubproject : jChangedContentOfProjectFile["subprojects"]) {
                    if (relativePath == cachedSubproject)
                        throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity,"Trying to add subproject(s) which already exists in cache");
                }
            }

            auto result = candidates.insert(relativePath);
            if(result.second == false)
                throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity, "Trying to add duplicated subprojects");
        }

        // if (candidates.size() > 1)
        //     for (size_t i = 0; i < candidates.size(); ++i)
        //         for (size_t ii = i + 1; ii < candidates.size(); ++ii)
        //             if (candidates[ii] == candidates[i])
        //                 throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity,"Trying to add duplicated subprojects");

        if (jChangedContentOfProjectFile.count("subprojects") == 0)
            jChangedContentOfProjectFile["subprojects"] = { };

        for(const auto& candidate : candidates)
            jChangedContentOfProjectFile["subprojects"].push_back(candidate);
        // TODO check for already added subprojects

    }

    void
    JsonFileSetLoader::addSubproject(const std::string & subp)
    {
        try {
            addSubprojects({subp});
        } catch (...) {
            throw;
        }
    }

    void
    JsonFileSetLoader::removeSubprojects(const vector<string>& subp)
    {

        if(loaded == false)
            throw FileSetRuntimeError(FileSetRuntimeError::NotLoaded, "Trying to remove subprojects on not loaded interface");

        //find duplicates
        vector<string> sorted = subp;
        std::sort(sorted.begin(), sorted.end());
        for (auto it = sorted.begin() + 1; it != sorted.end(); ++it)
            if (*it == *(it - 1))
                throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity,
                                          "Duplicate found in candidates to remove");

        //get reference for subprojects array
        auto& subprojects = jChangedContentOfProjectFile["subprojects"];

        //find nonexistent subprojects
        vector<json::iterator> candidates;
        for (const auto& path : subp) {
            const auto& found = std::find(subprojects.begin(), subprojects.end(), path);
            if (found == subprojects.end())
                throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity,
                                                      "Trying to remove nonexistent subproject(s)");
            else
                candidates.push_back(found);

        }

        //remove subprojects
        std::for_each(candidates.cbegin(), candidates.cend(), [&subprojects](json::iterator it){ subprojects.erase(it); });
        if (subprojects.empty()) {
            jChangedContentOfProjectFile.erase("subprojects");
        }

        //reload subproject loaders
        loadSubprojects();
    }

    void
    JsonFileSetLoader::removeSubproject(const std::string &s)
    {
        try {
            removeSubprojects({s});
        } catch (...) {
            throw;
        }
    }

    const json
    JsonFileSetLoader::checkProjectFileForErrors(ifstream& ifs) const
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

            if (j["subprojects"].is_array()) {
                if (j["subprojects"].at(0).is_string() == false)
                    return json::parse(error_code + "Wrong values type of subprojects key!\"}");
            } else if (!j["subprojects"].is_null() && !j["subprojects"].is_array()) {
                return json::parse(error_code + "Corrupted or absent subprojects key!\"}");
            }

            return j;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return json::parse(error_code + string("Error gathered from json API: ") + string(e.what()));
        }
    }

    void
    JsonFileSetLoader::loadSubprojects()
    {
        try {
            string pathHead, pathSub, nameOfSubproject;
            FileSetLoader * loaderOfSubproject = nullptr;
            char * cPathToProjectFile = nullptr, * dname = nullptr;
            cPathToProjectFile = strdup(pathToProjectFile.c_str());
            dname = dirname(cPathToProjectFile);
            pathHead = string(dname) + string("/");



            if (jChangedContentOfProjectFile.count("subprojects") == 1) { //prevent creating "subprojects" key with "null" value
                for (const auto& x: jChangedContentOfProjectFile["subprojects"]) {
                            pathSub = x.get<string>();
                            loaderOfSubproject = FileSetFactory::createFileSet(pathHead + pathSub);
                            loaderOfSubproject->open();
                            nameOfSubproject = loaderOfSubproject->getProjectName();
                            if (subprojects.count(nameOfSubproject) == 1) //because std::map
                                continue;
                            subprojects.emplace(nameOfSubproject, loaderOfSubproject);
                }
            }

            return;
            } catch (const FileSetRuntimeError& re) {
                throw FileSetRuntimeError(FileSetRuntimeError::BrokenSubproject, string(re.what()));
            }
    }

    FileSetLoader *
    JsonFileSetLoader::findSubprojectByPath(const std::string &path) const
    {
        for (auto it = subprojects.cbegin(); it != subprojects.cend(); ++it)
        {
            if (it->second->getPathToNode() == path)
                return it->second;
        }
        throw FileSetRuntimeError(FileSetRuntimeError::SubprojectsIncongruity, "Subproject not found by path search");
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
