//
// Created by ebeuque on 26/07/2021.
//

#ifndef OPENDBVIEWER_QTCOMPAT_H
#define OPENDBVIEWER_QTCOMPAT_H

#include <QtGlobal>

// Added in Qt 5.11
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
#define USE_QTFMHORIZONTALADVANCE
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define USE_QLIBRARYINFO_PATH
#endif

#endif //OPENDBVIEWER_QTCOMPAT_H
