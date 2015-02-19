#pragma once
#include <QtGlobal>

#if defined(LIBPROJ_LIBRARY)
#  define LIBPROJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBPROJSHARED_EXPORT Q_DECL_IMPORT
#endif

