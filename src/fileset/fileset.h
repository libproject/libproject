/*!
* \file
* \brief The header file with input interface specification
*
* Present file contains abstract interface class and realization of Factory
* pattern which creating interface instances on user demand
*/
#pragma once

#include <list>
#include <string>

namespace LibprojManager {
namespace Interface {

/*!
 * \brief The FileSetLoader class bases abstract interface which regulates
 * loading and gathering information from input sources
 *
 * It provides functions for:
 * opening input sources within error-reporting system covering,
 * getting file names of project in STL list<string> format,
 * getting name of project and path to root node in notions of input format
 */
class FileSetLoader
{
public:

    virtual ~FileSetLoader() { }

    virtual const std::string open() = 0;
    virtual const std::list<std::string> getFileNames() const = 0;
    virtual const std::string getProjectName() const = 0;
    virtual const std::string getPathToRootNode() const = 0;

};

class AbstractFileSetCreator {
public:

    virtual ~AbstractFileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const = 0;
};

class FileSetCreator : public AbstractFileSetCreator {
public:

    virtual ~FileSetCreator() { }

    virtual FileSetLoader * create(const std::string & pathToRootNode) const;
};

/*!
 * \brief The FileSetFactory class provides ability for creating instances
 * of input interface
 *
 * This factory is degenerate kind of such pattern because at this
 * moment this facility originates objects of only one kind
 */
class FileSetFactory
{
protected:
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
     * \brief createFileSet is public interface of factory facility
     * \param[in] pathToRootNode_ This parameter specifying path to input source
     * \return Pointer to interface instance
     */
    static FileSetLoader * createFileSet(const std::string & pathToRootNode_);
};



} // namespace Interface
} // namespace LibprojManager
