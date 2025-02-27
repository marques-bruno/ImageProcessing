#ifndef IMAGEPROCESSSINGPLUGIN_H
#define IMAGEPROCESSSINGPLUGIN_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_IMAGEPROCESSING_PLUGIN
#define SOFA_IMAGEPROCESSING_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_IMAGEPROCESSING_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif  // IMAGEPROCESSSINGPLUGIN_H
