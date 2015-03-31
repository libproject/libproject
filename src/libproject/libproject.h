/*!
* \file
* \brief The header file with input interface specification
*
* Present file contains abstract interface class and realization of Factory
* pattern which creates interface instances according to user demand
*/
#pragma once

#include <list>
#include <string>
#include <map>

namespace LibprojManager {
namespace Interface {

/*!
 * \brief Bases abstract interface which regulates and gathering
 * information from input sources
 *
 * It provides functions for:
 * opening input sources within error-report system,
 * getting file names of project in STL list<string> format,
 * getting name of project and path to root node in notions of input format
 */
class FileSetLoader
{
public:

    virtual ~FileSetLoader() { }

    virtual bool open() = 0;
    virtual const std::list<std::string> getFileNames() const = 0;
    virtual const std::string getProjectName() const = 0;
    virtual const std::string getPathToNode() const = 0;
    virtual const int countSubprojects() const = 0;
    virtual std::map<std::string, FileSetLoader *> getSubprojectLoaders() = 0;

};

/*!
 * \brief Just an abstract skeleton of Factory
 */
class AbstractFileSetCreator {
public:

    virtual ~AbstractFileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const = 0;
};

/*!
 * \brief Realization of skeleton of Factory
 */
class FileSetCreator : public AbstractFileSetCreator {
public:

    virtual ~FileSetCreator() { }

    /*!
     * \brief generic function of present pattern
     * \param[in] pathToRootNode specifying path to input source
     * \return Pointer to interface instance
     */
    virtual FileSetLoader * create(const std::string & pathToRootNode) const;
};

/*!
 * \brief Provides ability for creating instances of input interface
 *
 * This Factory is degenerate kind of such pattern because at this
 * moment this facility originates objects of only one kind
 */
class FileSetFactory
{
protected:
    /*!
     * \brief The Singleton for Factory skeleton instance
     *
     */
    class AbstractFileSetCreatorSingleton
    {
    private:
        AbstractFileSetCreatorSingleton() { }
    public:

        AbstractFileSetCreatorSingleton
        (const AbstractFileSetCreatorSingleton&) = delete;
        AbstractFileSetCreatorSingleton&
        operator=(AbstractFileSetCreatorSingleton&) = delete;

        static AbstractFileSetCreator& getCreator() {
            static FileSetCreator creator;
            return creator;
        }
    };

public:

    /*!
     * \brief Public interface of Factory facility
     * \param[in] pathToRootNode_ specifying path to input source
     * \return Pointer to interface instance
     */
    static FileSetLoader * createFileSet(const std::string & pathToRootNode_);
};



} // namespace Interface
} // namespace LibprojManager
