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
         * \return list<string> of filenames of project or empty list if project wasn't loaded
         */
        virtual const list<string> getFileNames() const;

        /*!
         * \brief Gives to user project name
         * \return project name in string format or empty string if project wasn't loaded
         */
        virtual const string getProjectName() const;

        /*!
         * \brief Gives to user path to root node
         * \return path to root node in string format or empty string if project wasn't loaded
         */
        virtual const string getPathToRootNode() const { return loaded ? pathToProjectFile : string(); }

    private:

        /*!
         * \brief check_json_for_errors
         * \return Json object within the content of project file or Json object
         * within single key "Error" string value of which contains description of error
         */
        const Json check_json_for_errors();

    };

    bool
    JsonFileSetLoader::open()
    {
        if (loaded) {
            throw ErrorInLoaderLogic(Code::PRLD, "Project alredy loaded!");
        }
        ifstream i(pathToProjectFile);
        if(!i) {
            throw ErrorDuringGettingProjectInfo(Code::ICRS,
                                                "Error with input stream!");
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
                throw ErrorDuringGettingProjectInfo(Code::ICRS,
                                                    jContentOfProjectFile["Error"].string_value());
            }
            else if (jContentOfProjectFile.is_null()) {
                loaded = false;
                throw ErrorDuringGettingProjectInfo(Code::UNKR,
                                                    "Unknown error gathered from json11 library!");
            }
            else {
                return loaded = true;
            }
        }
        else {
            loaded = false;
            throw ErrorDuringGettingProjectInfo(Code::ICRS,
                                                "Input stream didn't gave EOF marker!");
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
