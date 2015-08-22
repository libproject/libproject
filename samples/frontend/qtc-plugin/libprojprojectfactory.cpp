#include "libprojprojectfactory.h"

namespace LibprojManager {
namespace Interface {

AbstractProjectCreator *ProjectFactory::creator = new ProjectCreator();

ProjectFactory::~ProjectFactory() { delete creator; }

} // namespace Interface
} // namespace LibprojManager
