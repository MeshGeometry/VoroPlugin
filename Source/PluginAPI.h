#pragma once

#if defined (_WINDOWS)
    #if defined(IOPLUGIN_EXPORTS)
        #define IOPLUGIN __declspec(dllexport)
    #else
        #define IOPLUGIN __declspec(dllimport)
    #endif
#else
    #define IOPLUGIN
#endif

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#elif MAC
#define DLLEXPORT          EXTERNC __attribute__((visibility("default")))
#else
#define DLLEXPORT
#endif


