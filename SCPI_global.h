#ifndef SCPI_GLOBAL_H
#define SCPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCPI_LIBRARY)
#  define SCPISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCPISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCPI_GLOBAL_H
