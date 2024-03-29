#ifndef __aux_platform_h__
#define __aux_platform_h__

/*
 * Terra Informatica Sciter and HTMLayout Engines
 * http://terrainformatica.com/sciter, http://terrainformatica.com/htmlayout
 *
 * platform primitives.
 *
 * The code and information provided "as-is" without
 * warranty of any kind, either expressed or implied.
 *
 * (C) 2003-2013, Andrew Fedoniouk (andrew@terrainformatica.com)
 */


#if defined(_WIN32_WCE) || defined(UNDER_CE)
  #define PLATFORM_WINCE
  #define WINDOWS
#elif defined(WIN64) || defined(_WIN64) || defined(_M_X64)
  #define PLATFORM_DESKTOP
  #define WINDOWS
  #define X64BITS
#elif defined(WIN32) || defined(_WIN32)
  #define PLATFORM_DESKTOP
  #define WINDOWS
#elif defined(__APPLE__)
  #define PLATFORM_OSX
  #define OSX
  #define UTF8_CHARS // const char* is UTF8 sequence
  #ifdef __x86_64__
    #define X64BITS
  #endif
  #define POSIX
#elif defined( __linux__ )
  #define PLATFORM_LINUX
  #define LINUX
  #ifdef __x86_64__
    #define X64BITS
  #endif
  #define POSIX
  #define UTF8_CHARS // const char* is UTF8 sequence
#else
  #error "Unknown platform"
#endif

#if defined(WINDOWS)
  #define stricmp _stricmp
  #define wcsicmp _wcsicmp
#elif defined(POSIX)
  #define stricmp strcasecmp
  #define wcsicmp wcscasecmp
#endif

#endif
