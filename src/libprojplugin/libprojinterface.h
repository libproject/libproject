#pragma once
#include <QStringList>
#include <QFile>

namespace LibprojManager {
namespace Interface {


class FileSetLoader
{
public:
    FileSetLoader() { }
    virtual ~FileSetLoader() { }
protected:
    virtual bool readProjectFile(const std::string &) = 0;
    virtual const QStringList getFileNames() const = 0;
    virtual const QString getProjectName() const = 0;
};

} // namespace Interface
} // namespace LibprojManager

