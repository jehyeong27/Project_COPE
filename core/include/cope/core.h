#pragma once

// Export macro so the C# GUI (P/Invoke) and Python (ctypes) can load the DLL
#if defined(COPE_CORE_STATIC)
    #define COPE_API
#elif defined(_WIN32)
    #if defined(COPE_CORE_EXPORTS)
        #define COPE_API __declspec(dllexport)
    #else
        #define COPE_API __declspec(dllimport)
    #endif
#else
    #define COPE_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Returns the core library version string, e.g. "0.1.0"
COPE_API const char* cope_version(void);

#ifdef __cplusplus
}
#endif
