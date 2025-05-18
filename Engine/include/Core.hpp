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

#ifndef K_VERSION
#define K_VERSION 0
#endif
#ifndef K_VERSION_MINOR
#define K_VERSION_MINOR K_VERSION & 0x0f
#endif
#ifndef K_VERSION_MAJOR
#define K_VERSION_MAJOR K_VERSION >> 8 & 0x0f
#endif

#define KCOMPONENT(...)
#define KSYSTEM(...)
