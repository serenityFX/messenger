#ifndef __SCITER_X_DEBUG_H__
#define __SCITER_X_DEBUG_H__

//|
//| Sciter Inspector/Debug API
//|

#include "sciter-x-api.h"

typedef VOID (SCAPI * SciterInspectorPF)(HELEMENT root, ISciterAPI* papi); // the inspector entry function
typedef VOID (SCAPI * SciterWindowInspectorPF)(HWINDOW hwnd, ISciterAPI* papi); // the inspector entry function

inline  VOID    SCAPI SciterSetupDebugOutput ( HWINDOW hwndOrNull, LPVOID param, DEBUG_OUTPUT_PROC pfOutput) { return SAPI()->SciterSetupDebugOutput ( hwndOrNull,param,pfOutput); }
inline  BOOL    SCAPI SciterDebugSetupClient ( HWINDOW hwnd, LPVOID param, SCITER_DEBUG_BP_HIT_CB onBreakpointHit, SCITER_DEBUG_DATA_CB onDataRead) { return SAPI()->SciterDebugSetupClient ( hwnd, param, onBreakpointHit, onDataRead); }
inline  BOOL    SCAPI SciterDebugAddBreakpoint ( HWINDOW hwnd, LPCWSTR  fileUrl, UINT lineNo ) { return SAPI()->SciterDebugAddBreakpoint (hwnd,fileUrl,lineNo ); }
inline  BOOL    SCAPI SciterDebugRemoveBreakpoint ( HWINDOW hwnd, LPCWSTR fileUrl, UINT lineNo ) { return SAPI()->SciterDebugRemoveBreakpoint ( hwnd, fileUrl, lineNo ); }
inline  BOOL    SCAPI SciterDebugEnumBreakpoints ( HWINDOW hwnd, LPVOID param, SCITER_DEBUG_BREAKPOINT_CB  receiver) { return SAPI()->SciterDebugEnumBreakpoints (hwnd,param,receiver); }

#if defined(__cplusplus) && !defined( PLAIN_API_ONLY )

  namespace sciter
  {

    struct debug_output
    {
      debug_output(HWINDOW hwnd = 0)
      {
        setup_on(hwnd);
      }

      void setup_on(HWINDOW hwnd = 0)
      {
        ::SciterSetupDebugOutput(hwnd,this,_output_debug);
      }

      static VOID SC_CALLBACK _output_debug(LPVOID param, UINT subsystem, UINT severity, LPCWSTR text, UINT text_length)
      {
        static_cast<debug_output*>(param)->output((OUTPUT_SUBSYTEMS)subsystem,(OUTPUT_SEVERITY)severity, (const WCHAR*)text,text_length);
      }
      virtual void output( OUTPUT_SUBSYTEMS subsystem, OUTPUT_SEVERITY severity, const WCHAR* text, unsigned int text_length )
      {
        switch(severity)
        {
           case OS_INFO     : print("info:"); break;
           case OS_WARNING  : print("warning:"); break;
           case OS_ERROR    : print("error:"); break;
        }
        switch(subsystem)
        {
           case OT_DOM: print("DOM:"); break;
           case OT_CSSS: print("csss!:"); break;
           case OT_CSS:  print("css:"); break;
           case OT_TIS:  print("script:"); break;
        }
        if(text[text_length])
        {
          //unsigned n = wcslen(text);
          assert(false);
        }
        else
          print(text);
      }
#if defined(WINDOWS)
      virtual void print(const WCHAR* text) { OutputDebugStringW(text); }
      virtual void print(const char* text) { OutputDebugStringA(text); }
#elif defined(OSX) || defined(LINUX)
      virtual void print(const WCHAR* text) { std::cout << aux::w2a(text).c_str() << std::endl; }
      virtual void print(const char* text) { std::cout << text << std::endl; }
#endif

      void printf( const char* fmt, ... )
      {
        char buffer [ 2049 ];
        va_list args;
        va_start ( args, fmt );
#if defined(WINDOWS)
        int len = vsprintf_s( buffer, fmt, args );
#else
        int len = vsprintf( buffer, fmt, args );
#endif
        va_end ( args );
        buffer [ len ] = 0;
        buffer [ 2048 ] = 0;
        print(buffer);
      }

/*      void printf( const WCHAR* fmt, ... )
      {
        WCHAR buffer [ 2049 ];
        va_list args;
        va_start ( args, fmt );
#if defined(WINDOWS)
        int len = vswprintf_s( buffer, fmt, args );
#elif defined(OSX)
        int len = vswprintf( buffer, 2048, fmt, args );
#endif
        va_end ( args );
        buffer [ len ] = 0;
        buffer [ 2048 ] = 0;
        print(buffer);
      } */


    };

    struct debug_output_console: public debug_output
    {

#ifdef _WIN32_WCE
      FILE *f;
      ~debug_output_console()
      {
        fclose(f);
      }
#endif
      debug_output_console():debug_output()
      {
#ifdef _WINCE32_WCE
        f = 0;
#endif
      }
#if !defined(_WIN32_WCE)
      virtual void print(const WCHAR* text) /*override*/
      {
        print(aux::w2oem(text));
      }
      virtual void print(const char* text) /*override*/
      {
#if defined(WINDOWS)
        static bool initialized = false;
        if(!initialized)
        {
          AllocConsole();
#pragma warning( push )
#pragma warning(disable:4996)
          freopen("conin$", "r", stdin);
          freopen("conout$", "w", stdout);
          freopen("conout$", "w", stderr);
#pragma warning( pop )
          initialized = true;
        }
#endif
        while(text && *text)
          putchar(*text++);
      }
#else
      virtual void print(const WCHAR* text) /*override*/
      {
        if( !f )
          f = fopen("\\mosciter.log", "wb");
        fputws(text, f);
      }
      virtual void print(const char* text) /*override*/
      {
        if( !f )
          f = fopen("\\mosciter.log", "wb");
        fputs(text, f);
      }
#endif
    };
      
#if defined(OSX)
      
      inline void* load_inspector_dylib() {
      
  #if defined(TARGET_64)
#define INSPECTOR_DYLIB_NAME "inspector64.dylib"
  #else
#define INSPECTOR_DYLIB_NAME "inspector32.dylib"
  #endif
          pid_t pid;
          char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
          char folderpath[PROC_PIDPATHINFO_MAXSIZE];
          pid = getpid();
          proc_pidpath (pid, pathbuf, sizeof(pathbuf));
          realpath(pathbuf, folderpath);
          *strrchr(folderpath, '/') = '\0';
          
          void* lib_inspector_handle = dlopen("./" INSPECTOR_DYLIB_NAME, RTLD_LOCAL|RTLD_LAZY);
          if( !lib_inspector_handle )
          {
              const char* lookup_paths[] =
              {
                  "/" INSPECTOR_DYLIB_NAME,
                  "/../Frameworks/" INSPECTOR_DYLIB_NAME, // - (bundle folder)/Contents/Frameworks/
                  "/../../../" INSPECTOR_DYLIB_NAME // aside of bundle folder (SDK samples)
              };
              for( int n = 0; n < 3; ++n ) {
                  char tpath[PROC_PIDPATHINFO_MAXSIZE];
                  strcpy(tpath,folderpath);
                  strcat(tpath, lookup_paths[n]);
                  lib_inspector_handle = dlopen(tpath, RTLD_LOCAL|RTLD_LAZY);
                  if( lib_inspector_handle )
                      break;
              }
          }
          return lib_inspector_handle;
      }
      
#endif
          

    inline bool inspect(HELEMENT root)
    {
      static SciterInspectorPF pSciterInspect = 0;

      if(!pSciterInspect) {
#if defined(WINDOWS)
        HMODULE hinsp = LoadLibrary(
          #if defined(_WIN64)
            TEXT("inspector64.dll")
          #else
            TEXT("inspector32.dll")
          #endif
            );
        if(!hinsp)
          return false;
        pSciterInspect = (SciterInspectorPF)GetProcAddress(hinsp,"SciterInspector");
#elif defined(OSX)
        void* handle = load_inspector_dylib();
        if( !handle )
          return false;
        pSciterInspect = (SciterInspectorPF)dlsym(handle,"SciterInspector");
#endif

      }
      if(pSciterInspect)
      {
        pSciterInspect(root,SAPI());
        return true;
      }
      return false;
    }
    inline bool inspect(HWINDOW hwndView)
    {
      static SciterWindowInspectorPF pSciterInspect = 0;

      if(!pSciterInspect) {
#if defined(WINDOWS)
        HMODULE hinsp = LoadLibrary(
          #if defined(WIN64) | defined(_WIN64)
            TEXT("inspector64.dll")
          #else
            TEXT("inspector32.dll")
          #endif
            );
        if(!hinsp)
          return false;
        pSciterInspect = (SciterWindowInspectorPF)GetProcAddress(hinsp,"SciterWindowInspector");
#elif defined(OSX)
          void* handle = load_inspector_dylib();
          if( !handle )
              return false;
          pSciterInspect = (SciterWindowInspectorPF)dlsym(handle,"SciterWindowInspector");
#endif
      }
      if(pSciterInspect)
      {
        pSciterInspect(hwndView,SAPI());
        return true;
      }
      return false;
    }

    class script_debugger_client
    {
    public:

      typedef std::function<void(const WCHAR* fileUrl, unsigned lineNo)> breakpoint_receiver_t;

      script_debugger_client(): hSciterHost(0) {}

      void dbg_attachTo(HWINDOW hSciter) {
         assert(hSciterHost == 0);
         BOOL r = SciterDebugSetupClient(
                hSciterHost = hSciter,
                this,
                &_SCITER_DEBUG_BP_HIT_CB,
                &_SCITER_DEBUG_DATA_CB);
         assert(r); r = r;
      }

      void dbg_detach() {
         assert(hSciterHost != 0);
         BOOL r = SciterDebugSetupClient(
                hSciterHost,
                this,
                0,
                0);
         assert(r); r = r;
      }


      void dbg_addBreakpoint(const WCHAR* fileUrl, unsigned lineNo ) {
          SciterDebugAddBreakpoint(hSciterHost,LPCWSTR(fileUrl), int(lineNo));
      }

      void dbg_removeBreakpoint(const WCHAR* fileUrl, unsigned lineNo ) {
          SciterDebugRemoveBreakpoint(hSciterHost,LPCWSTR(fileUrl), int(lineNo));
      }

      void dbg_enumBreakpoints( std::function<void(const WCHAR* fileUrl, unsigned lineNo)> receiver ) {
          SciterDebugEnumBreakpoints(hSciterHost, &receiver, _SCITER_DEBUG_BREAKPOINT_CB);
      }

      virtual void     dbg_onDetached() = 0;
      virtual unsigned dbg_onBreakpointHit(const WCHAR* inFile, int onLine, const json::value& environmentData) = 0;
      virtual void     dbg_onDataReady(unsigned cmd, const json::value& data) = 0;

    protected:
      HWINDOW hSciterHost;

      static UINT SC_CALLBACK _SCITER_DEBUG_BP_HIT_CB(LPCWSTR inFile, UINT atLine, const VALUE* envData, LPVOID param) {
        script_debugger_client* self = static_cast<script_debugger_client*>(param);
        return self->dbg_onBreakpointHit((const WCHAR*)inFile,atLine,*envData);
      }
      static VOID SC_CALLBACK _SCITER_DEBUG_DATA_CB(UINT onCmd, const VALUE* data, LPVOID param) {
        script_debugger_client* self = static_cast<script_debugger_client*>(param);
        if(data)
        {
           self->dbg_onDataReady(onCmd,*data);
        }
        else
        {
           self->dbg_onDetached();
           self->hSciterHost = 0;
        }
      }
      static BOOL SC_CALLBACK _SCITER_DEBUG_BREAKPOINT_CB(LPCWSTR fileUrl, UINT lineNo, LPVOID param)
      {
        breakpoint_receiver_t* bpr = static_cast<breakpoint_receiver_t*>(param);
        (*bpr)((const WCHAR*)fileUrl,lineNo);
        return true;
      }

    };

  }

#endif

#endif
