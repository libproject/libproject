#ifndef LPROJ4_GLOBAL_H
#define LPROJ4_GLOBAL_H

#include <QtGlobal>

#if defined(LPROJ4_LIBRARY)
#  define LPROJ4SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LPROJ4SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LPROJ4_GLOBAL_H

