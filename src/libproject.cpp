/*!
* \file
* \brief The source file which contains realization of input interface
*
* Here is located JSON-like realization of abstract input interface
*/

#include "libproject/libproject.h"
#include <fstream>
#include <sstream>
#include <list>
#include "json.hpp"
#include "libproject/libproject_error.h"
#include <libgen.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>

using namespace LibprojManager::Interface;
using std::ifstream;
using std::ostringstream;
using std::string;
using std::list;
using nlohmann::json;
using namespace LibprojManager::Interface::Error;
using std::map;
using std::ofstream;
using std::vector;
using std::remove_if;
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
class JsonFileSetLoader : public FileSetLoader {
  const string pathToProjectFile;
  json jContentOfProjectFile;
  json jChangedContentOfProjectFile;
  FileSetLoader::Loaders subprojects;
  bool loaded; //! flag which becoming true condition when instance is busy

  // description constants:
  const char *const FILES_DESCR = "files";
  const char *const SUBPR_DESCR = "subprojects";
  const char *const AUTHOR_DESCR = "author";
  const char *const PROJECT_DESCR = "project";
  const char *const ERROR_DESCR = "Error";

public:
  /*!
   * \brief Constructs interface instance
   * \param[in] ProjectFile is std::string and it describes path to .libproject
   * file
   */
  JsonFileSetLoader(const string &ProjectFile)
      : pathToProjectFile(ProjectFile), loaded(false) {}

  /*!
   * \brief Reads file and parsing it
   * \return boolean true if file opened successfully
   */
  /*virtual*/ bool open();

  /*!
   * \brief Gives to user filepaths of files
   * \return return collection of files of project, in case of failure throws
   * exception
   */
  /*virtual*/ const FileSetLoader::Files getFileNames() const;

  /*!
   * \brief Saves changes of .libproject file to it
   */
  /*virtual*/ void save();

  /*!
   * \brief Gives to user project name
   * \return project name in string format or drops exception if project wasn't
   * loaded
   */
  /*virtual*/ const string getProjectName() const;

  /*!
   * \brief Gives to user path to node
   * \return path to node in string format or drops exception if project wasn't
   * loaded
   */
  /*virtual*/ const string getPathToNode() const {
    return loaded ? pathToProjectFile
                  : throw LoaderStateError(
                        LoaderStateError::GetPathToRootNodeWhileNotLoaded);
  }

  /*!
    * \brief Gives paths to subprojects to user
    * in relative path format (to project)
    * \return return collection of subprojects of project. Zero-sized vector
   * implied
    */
  /*virtual*/ const FileSetLoader::Subprojects getSubprojectsPaths() const;

  /*!
    * \brief Gives number of subprojects to user
    * \return number of subprojects. Zero implied
    */
  /*virtual*/ int countSubprojects() const;

  /*!
   * \brief Gives STL map with pointers to loaders
   * \return FileSetLoader::Loaders (map<string, FileSetLoader *>) of
   * subprojects associated with its names
   */
  /*virtual*/ FileSetLoader::Loaders getSubprojectLoaders();

  /*!
   * \brief This function performs adding existing subprojects which are present
   * on
   * filesystem to cache of .libproject file. But NOT saves it.
   * \param[in] Subprojects
   */
  /*virtual*/ void addSubprojects(const FileSetLoader::Subprojects &subp);

  /*!
   * \brief This function performs adding existing subproject which is present
   * on
   * filesystem to cache of .libproject file. But NOT saves it.
   * \param[in] std::string with path to subproject
   */
  /*virtual*/ void addSubproject(const FileSetLoader::Path &subp);

  /*!
   * \brief This function performs removing existing subprojects in cache or in
   * saved
   * .libproject file
   * \param[in] Subprojects
   */
  /*virtual*/ void removeSubprojects(const FileSetLoader::Subprojects &subp);

  /*!
   * \brief This function performs removing existing subproject in cache or in
   * saved
   * .libproject file
   * \param[in] std::string with path to subproject
   */
  /*virtual*/ void removeSubproject(const FileSetLoader::Path &s);

  /*!
   * \brief This function performs searching existing subproject loader by path
   * to
   * subproject
   * \param[in] std::string with path to subproject
   * \return loader of subproject or nullptr if there is no such subproject
   */
  /*virtual*/ FileSetLoader *
  findSubprojectByPath(const FileSetLoader::Path &path) const;

private:
  /*!
   * \brief checkProjectFileForErrors
   * \return Json object within the content of project file or Json object
   * within single key ERROR_DESCR string value of which contains description of
   * error
   */
  const json checkProjectFileForErrors(ifstream &) const;

  /*!
   * \brief loadSubprojects loads nested subprojects
   */
  void loadSubprojects();
};

bool JsonFileSetLoader::open() {
  if (loaded)
    throw LoaderStateError(LoaderStateError::LoadProjectWhileAlreadyLoaded);
  ifstream i(pathToProjectFile);
  if (!i)
    throw StreamError(StreamError::InputStreamError);

  jChangedContentOfProjectFile = jContentOfProjectFile =
      checkProjectFileForErrors(i); /// Checking JSON-data for consistentness,
                                    /// correctness and reading it

  if (jContentOfProjectFile.count(ERROR_DESCR) !=
      0) { /// Checking data for higher abstract errors - project level errors
    loaded = false;
    throw IncorrectSourceError(
        IncorrectSourceError::SourceError,
        jContentOfProjectFile[ERROR_DESCR].get<string>());
  } else {
    if (jContentOfProjectFile[SUBPR_DESCR]
            .is_array()) /// Checking project data for subprojects
      loadSubprojects(); /// If above is true loading subprojects
  }

  i.close();
  return loaded = true;
}

void JsonFileSetLoader::save() {
  if (jChangedContentOfProjectFile == jContentOfProjectFile)
    return;

  jContentOfProjectFile = jChangedContentOfProjectFile;
  ofstream o(pathToProjectFile);
  if (!o)
    throw StreamError(StreamError::OutputStreamError);
  o << std::setw(4) << jContentOfProjectFile;
  o.close();
  return;
}

const FileSetLoader::Files JsonFileSetLoader::getFileNames() const {
  if (loaded == false)
    throw LoaderStateError(LoaderStateError::GetFileNamesWhileNotLoaded);
  FileSetLoader::Files files;
  for (const auto &item : jContentOfProjectFile[FILES_DESCR]) {
    files.push_back(item.get<string>());
  }
  return files;
}

const string JsonFileSetLoader::getProjectName() const {
  if (loaded == false)
    throw LoaderStateError(LoaderStateError::GetProjectNameWhileNotLoaded);
  return jContentOfProjectFile[PROJECT_DESCR].get<string>();
}

const FileSetLoader::Subprojects
JsonFileSetLoader::getSubprojectsPaths() const {
  if (loaded == false)
    throw LoaderStateError(LoaderStateError::GetSubprojectsPathsWhileNotLoaded);

  if (jChangedContentOfProjectFile.count(SUBPR_DESCR) > 0) {
    auto &subprojects = jChangedContentOfProjectFile[SUBPR_DESCR];
    vector<string> paths;
    for (const auto &path : subprojects) {
      paths.push_back(path);
    }
    return paths;
  }
  throw SubprojectsError(
      SubprojectsError::GetSubprojectsPathsWhileThereAreNoSubprojects);
}

int JsonFileSetLoader::countSubprojects() const {
  if (loaded == false)
    throw LoaderStateError(LoaderStateError::CountSubprojectsWhileNotLoaded);
  return subprojects.empty() ? 0 : subprojects.size();
}

FileSetLoader::Loaders JsonFileSetLoader::getSubprojectLoaders() {
  if (loaded == false)
    throw LoaderStateError(
        LoaderStateError::GetSubprojectsLoadersWhileNotLoaded);
  return subprojects;
}

void JsonFileSetLoader::addSubprojects(const FileSetLoader::Subprojects &subp) {
  if (loaded == false)
    throw LoaderStateError(LoaderStateError::AddSubprojectsWhileNotLoaded);

  if (subp.empty())
    throw IncorrectLoaderBehaviour(
        IncorrectLoaderBehaviour::EmptySubprojectsContainerDetected);

  char *cPathToProjectFile, *dname;
  cPathToProjectFile = strdup(pathToProjectFile.c_str());
  dname = dirname(cPathToProjectFile);
  int whereRelativePathStarts =
      string(dname).length() + 1; // +1 because we need to skip "/" symbol
  set<string> candidates;
  for (const auto &sp : subp) {
    string relativePath = sp.substr(whereRelativePathStarts);

    ifstream checkSubprojectsStream;
    checkSubprojectsStream.open(string(dname) + string("/") + relativePath);
    json check = checkProjectFileForErrors(checkSubprojectsStream);
    if (check.count(ERROR_DESCR) != 0)
      throw SubprojectsError(SubprojectsError::AddBrokenSubproject);
    checkSubprojectsStream.close();

    if (jChangedContentOfProjectFile.count(SUBPR_DESCR) != 0) {
      for (const auto &cachedSubproject :
           jChangedContentOfProjectFile[SUBPR_DESCR]) {
        if (relativePath == cachedSubproject)
          throw SubprojectsError(SubprojectsError::AddExistingSubproject);
      }
    }

    auto result = candidates.insert(relativePath);
    if (result.second == false)
      throw SubprojectsError(SubprojectsError::AddEqualSubprojects);
  }

  if (jChangedContentOfProjectFile.count(SUBPR_DESCR) == 0)
    jChangedContentOfProjectFile[SUBPR_DESCR] = {};

  for (const auto &candidate : candidates)
    jChangedContentOfProjectFile[SUBPR_DESCR].push_back(candidate);
  // TODO check for already added subprojects
}

void JsonFileSetLoader::addSubproject(const std::string &subp) {
  try {
    addSubprojects({subp});
  } catch (...) {
    throw;
  }
}

void
JsonFileSetLoader::removeSubprojects(const FileSetLoader::Subprojects &subp) {

  if (loaded == false)
    throw LoaderStateError(LoaderStateError::RemoveSubprojectsWhileNotLoaded);

  // find duplicates
  FileSetLoader::Subprojects sorted = subp;
  std::sort(sorted.begin(), sorted.end());
  for (auto it = sorted.begin() + 1; it != sorted.end(); ++it)
    if (*it == *(it - 1))
      throw IncorrectLoaderBehaviour(
          IncorrectLoaderBehaviour::FoundDuplicateInCandidatesToRemove);

  // get reference for subprojects array
  auto &subprojects = jChangedContentOfProjectFile[SUBPR_DESCR];

  // find nonexistent subprojects
  vector<json::iterator> candidates;
  for (const auto &path : subp) {
    const auto &found = std::find(subprojects.begin(), subprojects.end(), path);
    if (found == subprojects.end())
      throw SubprojectsError(SubprojectsError::RemoveNonExistentSubproject);
    else
      candidates.push_back(found);
  }

  // remove subprojects
  std::for_each(candidates.cbegin(), candidates.cend(),
                [&subprojects](json::iterator it) { subprojects.erase(it); });
  if (subprojects.empty()) {
    jChangedContentOfProjectFile.erase(SUBPR_DESCR);
  }

  // reload subproject FileSetLoader::Loaders
  loadSubprojects();
}

void JsonFileSetLoader::removeSubproject(const std::string &s) {
  try {
    removeSubprojects({s});
  } catch (...) {
    throw;
  }
}

const json JsonFileSetLoader::checkProjectFileForErrors(ifstream &ifs) const {
  const string error_code =
      string("{\"") + string(ERROR_DESCR) + string("\" : \"");
  try {
    json j;
    j << ifs;
    if (ifs.eof() == false)
      return json::parse(error_code +
                         "Input stream didn't gave EOF marker!\"}");

    if (j.is_null())
      return json::parse(error_code + "Empty or broken file!\"}");

    if (j[PROJECT_DESCR].is_string() == false)
      return json::parse(error_code + "Corrupted or absent project key!\"}");

    if (j[FILES_DESCR].is_array() == false)
      return json::parse(error_code + "Corrupted or absent files key!\"}");
    else if (j[FILES_DESCR].at(0).is_string() == false)
      return json::parse(error_code + "Wrong values type of files key!\"}");

    if (j[SUBPR_DESCR].is_array()) { // bug in json lib
      if (j[SUBPR_DESCR].at(0).is_string() == false)
        return json::parse(error_code +
                           "Wrong values type of subprojects key!\"}");
    } else if (!j[SUBPR_DESCR].is_null()) {
      return json::parse(error_code +
                         "Corrupted or absent subprojects key!\"}");
    }

    return j;
  } catch (const std::exception &e) {
    throw StreamError(StreamError::SourceAPIError,
                      string("Error gathered from json API: ") +
                          string(e.what()));
  }
}

void JsonFileSetLoader::loadSubprojects() {
  try {
    string pathHead, pathSub, nameOfSubproject;
    FileSetLoader *loaderOfSubproject = nullptr;
    char *cPathToProjectFile = nullptr, *dname = nullptr;
    cPathToProjectFile = strdup(pathToProjectFile.c_str());
    dname = dirname(cPathToProjectFile);
    pathHead = string(dname) + string("/");

    if (jChangedContentOfProjectFile.count(SUBPR_DESCR) ==
        1) { // prevent creating SUBPR_DESCR key with "null" value
      for (const auto &x : jChangedContentOfProjectFile[SUBPR_DESCR]) {
        pathSub = x.get<string>();
        loaderOfSubproject = FileSetFactory::createFileSet(pathHead + pathSub);
        loaderOfSubproject->open();
        nameOfSubproject = loaderOfSubproject->getProjectName();
        if (subprojects.count(nameOfSubproject) == 1) // because std::map
          continue;
        subprojects.emplace(nameOfSubproject, loaderOfSubproject);
      }
    }

    return;
  } catch (const FileSetRuntimeError &re) {
    throw SubprojectsError(SubprojectsError::LoadBrokenSubproject,
                           string(re.what()));
  }
}

FileSetLoader *JsonFileSetLoader::findSubprojectByPath(const Path &path) const {
  if (jChangedContentOfProjectFile.find(SUBPR_DESCR) ==
          jChangedContentOfProjectFile.end() ||
      jChangedContentOfProjectFile[SUBPR_DESCR].size() == 0)
  /* so strange if condition due to strange behaviour of json api in such
     situation*/
  {
    return nullptr;
  }
  if (path == pathToProjectFile) {
    throw SubprojectsError(
        SubprojectsError::RepresentRootProjectPathAsOwnSubprojectPath);
  }
  for (auto it = subprojects.cbegin(); it != subprojects.cend(); ++it) {
    if (it->second->getPathToNode() == path)
      return it->second;
  }
  return nullptr;
}

FileSetLoader *FileSetCreator::create(const string &pathToRootNode) const {
  return new JsonFileSetLoader(pathToRootNode);
}

FileSetLoader *FileSetFactory::createFileSet(const string &pathToRootNode_) {
  return AbstractFileSetCreatorSingleton::getCreator().create(pathToRootNode_);
}

} // namespace Interface
} // namespace LibprojManager
