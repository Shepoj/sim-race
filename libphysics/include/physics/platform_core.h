#pragma once

#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#if (defined(WIN32) || defined(__WIN32__) || defined(_WIN32)) && !defined(SIMRACE_STATIC)
#if defined(SIMRACE_EXPORT)

#define SIMRACE_DLL __declspec(dllexport)

#else

#define SIMRACE_DLL __declspec(dllimport)

#endif
#else

#define SIMRACE_DLL

#endif


