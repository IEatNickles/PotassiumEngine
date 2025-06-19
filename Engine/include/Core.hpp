#pragma once

#ifdef K_PLATFORM_WINDOWS
#ifdef K_API_EXPORT
#define K_API __declspec(dllexport)
#else
#define K_API __declspec(dllimport)
#endif
#elif K_PLATFORM_LINUX
#ifdef K_API_EXPORT
#define K_API __attribute__((visibility("default")))
#else
#define K_API
#endif
#else
#define K_API
#endif

#ifndef K_VERSION_MINOR
#define K_VERSION_MINOR 0
#endif
#ifndef K_VERSION_MAJOR
#define K_VERSION_MAJOR 0
#endif
#define K_VERSION Version(K_VERSION_MINOR, K_VERSION_MAJOR)

#define KCOMPONENT(...)
#define KSYSTEM(...)
