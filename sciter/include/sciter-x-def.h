#ifndef __SCITER_X_DEF__
#define __SCITER_X_DEF__

/** \mainpage Terra Informatica Sciter engine.
 *
 * \section legal_sec In legalese
 *
 * The code and information provided "as-is" without
 * warranty of any kind, either expressed or implied.
 *
 * <a href="http://terrainformatica.com/Sciter">Sciter Home</a>
 *
 * (C) 2003-2006, Terra Informatica Software, Inc. and Andrew Fedoniouk
 *
 * \section structure_sec Structure of the documentation
 *
 * See <a href="files.html">Files</a> section.
 **/

/**\file sciter-x.h
 * Main include file.
 **/

#include "value.h"
#ifdef __cplusplus
  #include "aux-cvt.h"
  #include <iostream>
  #include <stdio.h>
  #include <stdarg.h>
  #include <wchar.h>
#endif




#define HAS_TISCRIPT

/** Resource data type.
 *  Used by SciterDataReadyAsync() function.
 **/
typedef enum SciterResourceType
{
  RT_DATA_HTML = 0,
  RT_DATA_IMAGE = 1,
  RT_DATA_STYLE = 2,
  RT_DATA_CURSOR = 3,
  RT_DATA_SCRIPT = 4,
  RT_DATA_RAW = 5,
} SciterResourceType;

#define  HAS_TISCRIPT // in sciter

#include "sciter-x-value.h"
#include "sciter-x-dom.h"

/**Get name of Sciter window class.
 *
 * \return \b LPCWSTR, name of Sciter window class.
 *         \b NULL if initialization of the engine failed, Direct2D or DirectWrite are not supported on the OS.
 *
 * Use this function if you wish to create unicode version of Sciter.
 * The returned name can be used in CreateWindow(Ex)W function.
 * You can use #SciterClassNameT macro.
 **/
EXTERN_C LPCWSTR SCAPI SciterClassName();

/**Returns major and minor version of Sciter engine.
 **/
EXTERN_C UINT  SCAPI SciterVersion(BOOL major);

enum
{
  LOAD_OK = 0,      // do default loading if data not set
  LOAD_DISCARD = 1, // discard request completely
  LOAD_DELAYED = 2, // data will be delivered later by the host application.
                    // Host application must call SciterDataReadyAsync(,,, requestId) on each LOAD_DELAYED request to avoid memory leaks.
};

/**Notifies that Sciter is about to download a referred resource.
 *
 * \param lParam #LPSCN_LOAD_DATA.
 * \return #LOAD_OK or #LOAD_DISCARD
 *
 * This notification gives application a chance to override built-in loader and
 * implement loading of resources in its own way (for example images can be loaded from
 * database or other resource). To do this set #SCN_LOAD_DATA::outData and
 * #SCN_LOAD_DATA::outDataSize members of SCN_LOAD_DATA. Sciter does not
 * store pointer to this data. You can call #SciterDataReady() function instead
 * of filling these fields. This allows you to free your outData buffer
 * immediately.
**/
#define SC_LOAD_DATA       0x01

/**This notification indicates that external data (for example image) download process
 * completed.
 *
 * \param lParam #LPSCN_DATA_LOADED
 *
 * This notifiaction is sent for each external resource used by document when
 * this resource has been completely downloaded. Sciter will send this
 * notification asynchronously.
 **/
#define SC_DATA_LOADED     0x02

/**This notification is sent when all external data (for example image) has been downloaded.
 *
 * This notification is sent when all external resources required by document
 * have been completely downloaded. Sciter will send this notification
 * asynchronously.
 **/
/* obsolete #define SC_DOCUMENT_COMPLETE 0x03
   use DOCUMENT_COMPLETE DOM event.
  */


/**This notification is sent on parsing the document and while processing
 * elements having non empty style.behavior attribute value.
 *
 * \param lParam #LPSCN_ATTACH_BEHAVIOR
 *
 * Application has to provide implementation of #sciter::behavior interface.
 * Set #SCN_ATTACH_BEHAVIOR::impl to address of this implementation.
 **/
#define SC_ATTACH_BEHAVIOR 0x04

/**This notification is sent when instance of the engine is destroyed.
 * It is always final notification.
 *
 * \param lParam #LPSCN_ENGINE_DESTROYED
 *
 **/
#define SC_ENGINE_DESTROYED 0x05

/**Posted notification.
 
 * \param lParam #LPSCN_POSTED_NOTIFICATION
 *
 **/
#define SC_POSTED_NOTIFICATION 0x06


/**Notification callback structure.
 **/
typedef struct SCITER_CALLBACK_NOTIFICATION
{
  UINT code; /**< [in] one of the codes above.*/
  HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
} SCITER_CALLBACK_NOTIFICATION;

typedef SCITER_CALLBACK_NOTIFICATION * LPSCITER_CALLBACK_NOTIFICATION;

typedef UINT SC_CALLBACK SciterHostCallback( LPSCITER_CALLBACK_NOTIFICATION pns, LPVOID callbackParam );

typedef SciterHostCallback * LPSciterHostCallback;


/**This structure is used by #SCN_LOAD_DATA notification.
 *\copydoc SCN_LOAD_DATA
 **/

typedef struct SCN_LOAD_DATA
{
    UINT code; /**< [in] one of the codes above.*/
    HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/

    LPCWSTR  uri;              /**< [in] Zero terminated string, fully qualified uri, for example "http://server/folder/file.ext".*/

    LPCBYTE  outData;          /**< [in,out] pointer to loaded data to return. if data exists in the cache then this field contain pointer to it*/
    UINT     outDataSize;      /**< [in,out] loaded data size to return.*/
    UINT     dataType;         /**< [in] SciterResourceType */

    LPVOID   requestId;        /**< [in] request id that needs to be passed as is to the SciterDataReadyAsync call */

    HELEMENT principal;
    HELEMENT initiator;
} SCN_LOAD_DATA;

typedef SCN_LOAD_DATA*  LPSCN_LOAD_DATA;

/**This structure is used by #SCN_DATA_LOADED notification.
 *\copydoc SCN_DATA_LOADED
 **/
typedef struct SCN_DATA_LOADED
{
    UINT code; /**< [in] one of the codes above.*/
    HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/

    LPCWSTR  uri;              /**< [in] zero terminated string, fully qualified uri, for example "http://server/folder/file.ext".*/
    LPCBYTE  data;             /**< [in] pointer to loaded data.*/
    UINT    dataSize;         /**< [in] loaded data size (in bytes).*/
    UINT     dataType;         /**< [in] SciterResourceType */
    UINT     status;           /**< [in]
                                         status = 0 (dataSize == 0) - unknown error.
                                         status = 100..505 - http response status, Note: 200 - OK!
                                         status > 12000 - wininet error code, see ERROR_INTERNET_*** in wininet.h
                                 */
} SCN_DATA_LOADED;

typedef SCN_DATA_LOADED * LPSCN_DATA_LOADED;

/**This structure is used by #SCN_ATTACH_BEHAVIOR notification.
 *\copydoc SCN_ATTACH_BEHAVIOR **/
typedef struct SCN_ATTACH_BEHAVIOR
{
    UINT code; /**< [in] one of the codes above.*/
    HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/

    HELEMENT element;          /**< [in] target DOM element handle*/
    LPCSTR   behaviorName;     /**< [in] zero terminated string, string appears as value of CSS behavior:"???" attribute.*/

    ElementEventProc* elementProc;    /**< [out] pointer to ElementEventProc function.*/
    LPVOID            elementTag;     /**< [out] tag value, passed as is into pointer ElementEventProc function.*/

} SCN_ATTACH_BEHAVIOR;
typedef SCN_ATTACH_BEHAVIOR* LPSCN_ATTACH_BEHAVIOR;

/**This structure is used by #SC_ENGINE_DESTROYED notification.
 *\copydoc SCN_ENGINE_DESTROYED **/
typedef struct SCN_ENGINE_DESTROYED
{
    UINT code; /**< [in] one of the codes above.*/
    HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
} SCN_ENGINE_DESTROYED;

typedef SCN_ENGINE_DESTROYED* LPSCN_ENGINE_DESTROYED;

/**This structure is used by #SC_ENGINE_DESTROYED notification.
 *\copydoc SCN_ENGINE_DESTROYED **/
typedef struct SCN_POSTED_NOTIFICATION
{
    UINT      code; /**< [in] one of the codes above.*/
    HWINDOW   hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
    UINT_PTR  wparam;
    UINT_PTR  lparam;
    UINT_PTR  lreturn;
} SCN_POSTED_NOTIFICATION;

typedef SCN_POSTED_NOTIFICATION* LPSCN_POSTED_NOTIFICATION;

#include "sciter-x-behavior.h"

/**This function is used in response to SCN_LOAD_DATA request.
 *
 * \param[in] hwnd \b HWINDOW, Sciter window handle.
 * \param[in] uri \b LPCWSTR, URI of the data requested by Sciter.
 * \param[in] data \b LPBYTE, pointer to data buffer.
 * \param[in] dataLength \b UINT, length of the data in bytes.
 * \return \b BOOL, TRUE if Sciter accepts the data or \c FALSE if error occured
 * (for example this function was called outside of #SCN_LOAD_DATA request).
 *
 * \warning If used, call of this function MUST be done ONLY while handling
 * SCN_LOAD_DATA request and in the same thread. For asynchronous resource loading
 * use SciterDataReadyAsync
 **/
EXTERN_C BOOL SCAPI SciterDataReady(HWINDOW hwnd,LPCWSTR uri,LPCBYTE data, UINT dataLength);

/**Use this function outside of SCN_LOAD_DATA request. This function is needed when you
 * you have your own http client implemented in your application.
 *
 * \param[in] hwnd \b HWINDOW, Sciter window handle.
 * \param[in] uri \b LPCWSTR, URI of the data requested by Sciter.
 * \param[in] data \b LPBYTE, pointer to data buffer.
 * \param[in] dataLength \b UINT, length of the data in bytes.
 * \param[in] requestId \b LPVOID, SCN_LOAD_DATA requestId.
 * \return \b BOOL, TRUE if Sciter accepts the data or \c FALSE if error occured
 **/

EXTERN_C BOOL SCAPI SciterDataReadyAsync(HWINDOW hwnd,LPCWSTR uri, LPCBYTE data, UINT dataLength,
                                         LPVOID requestId);

#ifdef WINDOWS

/**Sciter Window Proc.*/
EXTERN_C LRESULT SCAPI SciterProc(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**Sciter Window Proc without call of DefWindowProc.*/
EXTERN_C LRESULT SCAPI SciterProcND(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL* pbHandled);

#endif


/**Load HTML file.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] filename \b LPCWSTR, File name of an HTML file.
 * \return \b BOOL, \c TRUE if the text was parsed and loaded successfully, \c FALSE otherwise.
 **/
EXTERN_C BOOL SCAPI     SciterLoadFile(HWINDOW hWndSciter, LPCWSTR filename);

/**Load HTML from in memory buffer with base.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] html \b LPCBYTE, Address of HTML to load.
 * \param[in] htmlSize \b UINT, Length of the array pointed by html parameter.
 * \param[in] baseUrl \b LPCWSTR, base URL. All relative links will be resolved against
 *                                this URL.
 * \return \b BOOL, \c TRUE if the text was parsed and loaded successfully, FALSE otherwise.
 **/
EXTERN_C BOOL SCAPI     SciterLoadHtml(HWINDOW hWndSciter, LPCBYTE html, UINT htmlSize, LPCWSTR baseUrl);

/**Set \link #SCITER_NOTIFY() notification callback function \endlink.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] cb \b SCITER_NOTIFY*, \link #SCITER_NOTIFY() callback function \endlink.
 * \param[in] cbParam \b LPVOID, parameter that will be passed to \link #SCITER_NOTIFY() callback function \endlink as vParam paramter.
 **/
EXTERN_C VOID SCAPI     SciterSetCallback(HWINDOW hWndSciter, LPSciterHostCallback cb, LPVOID cbParam);

/**Set Master style sheet.
  See: http://www.terrainformatica.com/sciter/master_ss.css.txt
  Or resource section of the library for "master-css" HTML resource.
 *
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 **/

EXTERN_C BOOL SCAPI     SciterSetMasterCSS(LPCBYTE utf8, UINT numBytes);

/**Append Master style sheet.
  This function appends intrinsic style sheet by custom styles.
  See: http://www.terrainformatica.com/wiki/h-smile:built-in-behaviors:master_style_sheet
 *
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 *
 **/

EXTERN_C BOOL SCAPI     SciterAppendMasterCSS(LPCBYTE utf8, UINT numBytes);

/**Set (reset) style sheet of current document.
 Will reset styles for all elements according to given CSS (utf8)
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 **/

EXTERN_C BOOL SCAPI     SciterSetCSS(HWINDOW hWndSciter, LPCBYTE utf8, UINT numBytes, LPCWSTR baseUrl, LPCWSTR mediaType);

/**Set media type of this sciter instance.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] mediaType \b LPCWSTR, media type name.
 *
 * For example media type can be "handheld", "projection", "screen", "screen-hires", etc.
 * By default sciter window has "screen" media type.
 *
 * Media type name is used while loading and parsing style sheets in the engine so
 * you should call this function *before* loading document in it.
 *
 **/

EXTERN_C BOOL SCAPI     SciterSetMediaType(HWINDOW hWndSciter, LPCWSTR mediaType);

/**Set media variables of this sciter instance.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] mediaVars \b SCITER_VALUE, map that contains name/value pairs - media variables to be set.
 *
 * For example media type can be "handheld:true", "projection:true", "screen:true", etc.
 * By default sciter window has "screen:true" and "desktop:true"/"handheld:true" media variables.
 *
 * Media variables can be changed in runtime. This will cause styles of the document to be reset.
 *
 **/

EXTERN_C BOOL SCAPI     SciterSetMediaVars(HWINDOW hWndSciter, const SCITER_VALUE *mediaVars);

EXTERN_C UINT SCAPI     SciterGetMinWidth(HWINDOW hWndSciter);
EXTERN_C UINT SCAPI     SciterGetMinHeight(HWINDOW hWndSciter, UINT width);

EXTERN_C BOOL SCAPI     SciterCall(HWINDOW hWnd, LPCSTR functionName, UINT argc, const SCITER_VALUE* argv, SCITER_VALUE* retval);
// evalue script in context of current document
EXTERN_C BOOL SCAPI     SciterEval( HWINDOW hwnd, LPCWSTR script, UINT scriptLength, SCITER_VALUE* pretval);

/**Update pending changes in Sciter window.
 *
 * \param[in] hwnd \b HWINDOW, Sciter window handle.
 *
 **/
EXTERN_C VOID SCAPI     SciterUpdateWindow(HWINDOW hwnd);

/** Try to translate message that sciter window is interested in.
 *
 * \param[in,out] lpMsg \b MSG*, address of message structure that was passed before to ::DispatchMessage(), ::PeekMessage().
 *
 * SciterTranslateMessage has the same meaning as ::TranslateMessage() and should be called immediately before it.
 * Example:
 *
 *   if( !SciterTranslateMessage(&msg) )
 *      TranslateMessage(&msg);
 *
 * ATTENTION!: SciterTranslateMessage call is critical for popup elements in MoSciter.
 *             On Desktop versions of the Sciter this function does nothing so can be ommited.
 *
 **/

#ifdef WINDOWS
EXTERN_C BOOL SCAPI SciterTranslateMessage(MSG* lpMsg);
#endif

/**Set various options.
 *
 * \param[in] hWnd \b HWINDOW, Sciter window handle.
 * \param[in] option \b UINT, id of the option, one of SCITER_RT_OPTIONS
 * \param[in] option \b UINT, value of the option.
 *
 **/

enum SCRIPT_RUNTIME_FEATURES
{
  ALLOW_FILE_IO = 0x00000001,
  ALLOW_SOCKET_IO = 0x00000002,
  ALLOW_EVAL = 0x00000004,
  ALLOW_SYSINFO = 0x00000008
};

enum GFX_LAYER
{
  GFX_LAYER_GDI      = 1,
  GFX_LAYER_WARP     = 2,
  GFX_LAYER_D2D      = 3,
  GFX_LAYER_AUTO     = 0xFFFF,
};

enum SCITER_RT_OPTIONS
{
   SCITER_SMOOTH_SCROLL = 1,      // value:TRUE - enable, value:FALSE - disable, enabled by default
   SCITER_CONNECTION_TIMEOUT = 2, // value: milliseconds, connection timeout of http client
   SCITER_HTTPS_ERROR = 3,        // value: 0 - drop connection, 1 - use builtin dialog, 2 - accept connection silently
   SCITER_FONT_SMOOTHING = 4,     // value: 0 - system default, 1 - no smoothing, 2 - std smoothing, 3 - clear type

   SCITER_TRANSPARENT_WINDOW = 6, // Windows Aero support, value:
                                  // 0 - normal drawing,
                                  // 1 - window has transparent background after calls DwmExtendFrameIntoClientArea() or DwmEnableBlurBehindWindow().
   SCITER_SET_GPU_BLACKLIST  = 7, // hWnd = NULL,
                                  // value = LPCBYTE, json - GPU black list, see: gpu-blacklist.json resource.
   SCITER_SET_SCRIPT_RUNTIME_FEATURES = 8, // value - combination of SCRIPT_RUNTIME_FEATURES flags.
   SCITER_SET_GFX_LAYER = 9,      // hWnd = NULL, value - GFX_LAYER
   SCITER_SET_DEBUG_MODE = 10,    // hWnd, value - TRUE/FALSE
   SCITER_SET_UX_THEMING = 11     // hWnd = NULL, value - BOOL, TRUE - the engine will use "unisex" theme that is common for all platforms. 
                                  // That UX theme is not using OS primitives for rendering input elements. Use it if you want exactly
                                  // the same (modulo fonts) look-n-feel on all platforms.
};

EXTERN_C BOOL SCAPI SciterSetOption(HWINDOW hWnd, UINT option, UINT_PTR value );

/**Get current pixels-per-inch metrics of the Sciter window
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[out] px \b PUINT, get ppi in horizontal direction.
 * \param[out] py \b PUINT, get ppi in vertical direction.
 *
 **/

EXTERN_C VOID SCAPI SciterGetPPI(HWINDOW hWndSciter, UINT* px, UINT* py);

/**Get "expando" of the view object
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[out] pval \b VALUE*, expando as sciter::value.
 *
 **/

EXTERN_C BOOL SCAPI SciterGetViewExpando( HWINDOW hwnd, VALUE* pval );

typedef struct URL_DATA
{
  LPCSTR             requestedUrl;   // requested URL
  LPCSTR             realUrl;        // real URL data arrived from (after possible redirections)
  SciterResourceType requestedType;  // requested data category: html, script, image, etc.
  LPCSTR             httpHeaders;    // if any
  LPCSTR             mimeType;       // mime type reported by server (if any)
  LPCSTR             encoding;       // data encoding (if any)
  LPCBYTE            data;
  UINT              dataLength;
} URL_DATA;

typedef VOID SC_CALLBACK URL_DATA_RECEIVER( const URL_DATA* pUrlData, LPVOID param );


/** Get url resource data received by the engine
 *  Note: this function really works only if the engine is set to debug mode.
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] receiver \b URL_DATA_RECEIVER, address of reciver callback.
 * \param[in] param \b LPVOID, param passed to callback as it is.
 * \param[in] url \b LPCSTR, optional, url of the data. If not provided the engine will list all loaded resources
 * \return \b BOOL, \c TRUE if receiver is called at least once, FALSE otherwise.
 *
 **/

EXTERN_C BOOL SCAPI SciterEnumUrlData(HWINDOW hWndSciter, URL_DATA_RECEIVER* receiver, LPVOID param, LPCSTR url);


#ifdef WINDOWS

/**Render document to ID2D1RenderTarget
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] ID2D1RenderTarget \b prt, Direct2D render target.
 * \return \b BOOL, \c TRUE if hBmp is 24bpp or 32bpp, FALSE otherwise.
 *
 **/

EXTERN_C BOOL SCAPI SciterRenderD2D(HWINDOW hWndSciter, ID2D1RenderTarget* prt);

/** Obtain pointer to ID2D1Factory instance used by the engine:
 *
 * \param[in] ID2D1Factory \b **ppf, address of variable receiving pointer of ID2D1Factory.
 * \return \b BOOL, \c TRUE if parameters are valid and *ppf was set by valid pointer.
 *
 * NOTE 1: ID2D1Factory returned by the function is "add-refed" - it is your responsibility to call Release() on it.
 * NOTE 2: *ppf variable shall be initialized to NULL before calling the function.
 *
 **/

EXTERN_C BOOL SCAPI     SciterD2DFactory(ID2D1Factory ** ppf);

/** Obtain pointer to IDWriteFactory instance used by the engine:
 *
 * \param[in] IDWriteFactory \b **ppf, address of variable receiving pointer of IDWriteFactory.
 * \return \b BOOL, \c TRUE if parameters are valid and *ppf was set by valid pointer.
 *
 * NOTE 1: IDWriteFactory returned by the function is "add-refed" - it is your responsibility to call Release() on it.
 * NOTE 2: *ppf variable shall be initialized to NULL before calling the function.
 *
 **/

EXTERN_C BOOL SCAPI     SciterDWFactory(IDWriteFactory ** ppf);

/** Get graphics capabilities of the system
 *
 * \pcaps[in] LPUINT \b pcaps, address of variable receiving:
 *                             0 - no compatible graphics found;
 *                             1 - compatible graphics found but Direct2D will use WARP driver (software emulation);
 *                             2 - Direct2D will use hardware backend (best performance);
 * \return \b BOOL, \c TRUE if pcaps is valid pointer.
 *
 **/

#endif

EXTERN_C BOOL SCAPI     SciterGraphicsCaps(LPUINT pcaps);


/** Set sciter home url.
 *  home url is used for resolving sciter: urls
 *  If you will set it like SciterSetHomeURL(hwnd,"http://sciter.com/modules/")
 *  then <script src="sciter:lib/root-extender.tis"> will load
 *  root-extender.tis from http://sciter.com/modules/lib/root-extender.tis
 *
 * \param[in] hWndSciter \b HWINDOW, Sciter window handle.
 * \param[in] baseUrl \b LPCWSTR, URL of sciter home.
 *
 **/

EXTERN_C BOOL SCAPI     SciterSetHomeURL(HWINDOW hWndSciter, LPCWSTR baseUrl);

#if defined(OSX)
  EXTERN_C HWINDOW SCAPI  SciterCreateNSView( LPRECT frame ); // returns NSView*
  typedef LPVOID SciterWindowDelegate; // Obj-C id, NSWindowDelegate and NSResponder
#elif defined(WINDOWS)
  typedef BOOL SC_CALLBACK SciterWindowDelegate(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LPVOID pParam, LRESULT* pResult);
#elif defined(LINUX)
  typedef LPVOID SciterWindowDelegate;
#endif

enum SCITER_CREATE_WINDOW_FLAGS {
   SW_CHILD      = (1 << 0), // child window only, if this flag is set all other flags ignored
   SW_TITLEBAR   = (1 << 1), // toplevel window, has titlebar
   SW_RESIZEABLE = (1 << 2), // has resizeable frame
   SW_TOOL       = (1 << 3), // is tool window
   SW_CONTROLS   = (1 << 4), // has minimize / maximize buttons
   SW_GLASSY     = (1 << 5), // glassy window ( DwmExtendFrameIntoClientArea on windows )
   SW_ALPHA      = (1 << 6), // transparent window ( e.g. WS_EX_LAYERED on Windows )
   SW_MAIN       = (1 << 7), // main window of the app, will terminate the app on close
   SW_POPUP      = (1 << 8), // the window is created as topmost window.
   SW_ENABLE_DEBUG = (1 << 9), // make this window inspector ready
   SW_OWNS_VM      = (1 << 10), // it has its own script VM
};

/** Create sciter window.
 *  On Windows returns HWND of either top-level or child window created.
 *  On OS X returns NSView* of either top-level window or child view .
 *
 * \param[in] creationFlags \b SCITER_CREATE_WINDOW_FLAGS, creation flags.
 * \param[in] frame \b LPRECT, window frame position and size.
 * \param[in] delegate \b SciterWindowDelegate, either partial WinProc implementation or thing implementing NSWindowDelegate protocol.
 * \param[in] delegateParam \b LPVOID, optional param passed to SciterWindowDelegate.
 * \param[in] parent \b HWINDOW, optional parent window.
 *
 **/
EXTERN_C HWINDOW SCAPI  SciterCreateWindow( UINT creationFlags,
                                            LPRECT frame,
                                            SciterWindowDelegate* delegate,
                                            LPVOID delegateParam,
                                            HWINDOW parent);

/** SciterSetupDebugOutput - setup debug output function.
 *
 *  This output function will be used for reprting problems
 *  found while loading html and css documents.
 *
 **/

enum OUTPUT_SUBSYTEMS
{
   OT_DOM = 0,       // html parser & runtime
   OT_CSSS,          // csss! parser & runtime
   OT_CSS,           // css parser
   OT_TIS,           // TIS parser & runtime
};
enum OUTPUT_SEVERITY
{
  OS_INFO,
  OS_WARNING,
  OS_ERROR,
};

typedef VOID (SC_CALLBACK* DEBUG_OUTPUT_PROC)(LPVOID param, UINT subsystem /*OUTPUT_SUBSYTEMS*/, UINT severity, LPCWSTR text, UINT text_length);

EXTERN_C VOID SCAPI SciterSetupDebugOutput(
                HWINDOW                  hwndOrNull,// HWINDOW or null if this is global output handler
                LPVOID                param,     // param to be passed "as is" to the pfOutput
                DEBUG_OUTPUT_PROC     pfOutput   // output function, output stream alike thing.
                );

enum TISCRIPT_DEBUG_COMMANDS
{
  SCRIPT_DEBUG_CONTINUE = 1,
  SCRIPT_DEBUG_STEP_INTO,
  SCRIPT_DEBUG_STEP_OVER,
  SCRIPT_DEBUG_STEP_OUT,
  //SCRIPT_DEBUG_GET_CONTEXT, // will cause onDataReady() call
};

enum TISCRIPT_DEBUG_DATA_REQUESTS
{
  SCRIPT_DEBUG_CONTEXT = 0x100,
  SCRIPT_DEBUG_NAMESPACES = 0x200,
  SCRIPT_DEBUG_STACKTRACE = 0x300,
};

typedef UINT (SC_CALLBACK* SCITER_DEBUG_BP_HIT_CB)(LPCWSTR inFile, UINT atLine,  const VALUE* envData, LPVOID param); // breakpoint hit event receiver
typedef VOID (SC_CALLBACK* SCITER_DEBUG_DATA_CB)(UINT onCmd, const VALUE* data, LPVOID param); // requested data ready receiver

EXTERN_C BOOL SCAPI SciterDebugSetupClient(
                HWINDOW                        hwnd,      // HWINDOW of the sciter
                LPVOID                      param,     // param to be passed "as is" to these functions:
                SCITER_DEBUG_BP_HIT_CB      onBreakpointHit,  // breakpoint hit event receiver
                SCITER_DEBUG_DATA_CB        onDataRead        // receiver of requested data
              );

EXTERN_C BOOL SCAPI SciterDebugAddBreakpoint(
                HWINDOW     hwnd,      // HWINDOW of the sciter
                LPCWSTR  fileUrl,
                UINT     lineNo
              );

EXTERN_C BOOL SCAPI SciterDebugRemoveBreakpoint(
                HWINDOW     hwnd,      // HWINDOW of the sciter
                LPCWSTR  fileUrl,
                UINT     lineNo
              );

typedef BOOL (SC_CALLBACK* SCITER_DEBUG_BREAKPOINT_CB)(LPCWSTR fileUrl, UINT lineNo, LPVOID param);

EXTERN_C BOOL SCAPI SciterDebugEnumBreakpoints(
                HWINDOW                        hwnd,      // HWINDOW of the sciter
                LPVOID                      param,     // param to be passed "as is" to the pfOutput
                SCITER_DEBUG_BREAKPOINT_CB  receiver
              );


#endif

