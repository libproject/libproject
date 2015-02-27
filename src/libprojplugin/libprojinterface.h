#pragma once
#include <QStringList>
#include <QFile>

namespace LibprojManager {
namespace Interface {


class InputInterface
{
public:
    InputInterface() { }
    virtual ~InputInterface() { }
protected:
    virtual bool readFile(QFile&) = 0;
    virtual const QStringList & getFileNames() const = 0;
    virtual const QString & getProjectName() const = 0;
};

} // namespace Interface
} // namespace LibprojManager

