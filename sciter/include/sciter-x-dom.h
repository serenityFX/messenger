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

/**\file
 * \brief \link sciter_dom.h DOM \endlink C++ wrapper
 **/

#ifndef __sciter_dom_h__
#define __sciter_dom_h__

#include <assert.h>
#include <stdio.h> // for vsnprintf

#if defined(__cplusplus) && !defined( PLAIN_API_ONLY )
  namespace html
  {
    struct element;
    struct node;
  }
  namespace tool {
    class sar;
  }
  typedef html::element* HELEMENT;
  /**DOM node handle.*/
  typedef html::node* HNODE;
  /**DOM range handle.*/
  typedef void*  HRANGE;
  typedef struct hposition { HNODE hn; INT pos; } HPOSITION;
  typedef tool::sar* HSARCHIVE;
#else
  /**DOM element handle.*/
  typedef void*  HELEMENT;
  /**DOM node handle.*/
  typedef void*  HNODE;
  /**DOM range handle.*/
  typedef void*  HRANGE;
  typedef struct hposition { HNODE hn; INT pos; } HPOSITION;
#endif

//#include <string>
#include "tiscript.h"

#pragma warning(disable:4786) //identifier was truncated...
#pragma warning(disable:4100) //unreferenced formal parameter

#pragma once

/**Type of the result value for Sciter DOM functions.
 * Possible values are:
 * - \b SCDOM_OK - function completed successfully
 * - \b SCDOM_INVALID_HWND - invalid HWINDOW
 * - \b SCDOM_INVALID_HANDLE - invalid HELEMENT
 * - \b SCDOM_PASSIVE_HANDLE - attempt to use HELEMENT which is not marked by
 *   #Sciter_UseElement()
 * - \b SCDOM_INVALID_PARAMETER - parameter is invalid, e.g. pointer is null
 * - \b SCDOM_OPERATION_FAILED - operation failed, e.g. invalid html in
 *   #SciterSetElementHtml()
 **/

#define SCDOM_RESULT INT

#define SCDOM_OK 0
#define SCDOM_INVALID_HWND 1
#define SCDOM_INVALID_HANDLE 2
#define SCDOM_PASSIVE_HANDLE 3
#define SCDOM_INVALID_PARAMETER 4
#define SCDOM_OPERATION_FAILED 5
#define SCDOM_OK_NOT_HANDLED (-1)

typedef struct METHOD_PARAMS METHOD_PARAMS;
typedef struct REQUEST_PARAM { LPCWSTR name; LPCWSTR value; } REQUEST_PARAM;

EXTERN_C SCDOM_RESULT SCAPI Sciter_UseElement(HELEMENT he);

/**Marks DOM object as unused (a.k.a. Release).
 * Get handle of every element's child element.
 * \param[in] he \b #HELEMENT
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * Application should call this function when it does not need element's
 * handle anymore.
 * \sa #Sciter_UseElement()
 **/
EXTERN_C SCDOM_RESULT SCAPI Sciter_UnuseElement(HELEMENT he);

/**Get root DOM element of HTML document.
 * \param[in] hwnd \b HWINDOW, Sciter window for which you need to get root
 * element
 * \param[out ] phe \b #HELEMENT*, variable to receive root element
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * Root DOM object is always a 'HTML' element of the document.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetRootElement(HWINDOW hwnd, HELEMENT *phe);

/**Get focused DOM element of HTML document.
 * \param[in] hwnd \b HWINDOW, Sciter window for which you need to get focus
 * element
 * \param[out ] phe \b #HELEMENT*, variable to receive focus element
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * phe can have null value (0).
 *
 * COMMENT: To set focus on element use SciterSetElementState(STATE_FOCUS,0)
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetFocusElement(HWINDOW hwnd, HELEMENT *phe);

/**Find DOM element by coordinate.
 * \param[in] hwnd \b HWINDOW, Sciter window for which you need to find
 * elementz
 * \param[in] pt \b POINT, coordinates, window client area relative.
 * \param[out ] phe \b #HELEMENT*, variable to receive found element handle.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * If element was not found then *phe will be set to zero.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterFindElement(HWINDOW hwnd, POINT pt, HELEMENT* phe);

/**Get number of child elements.
 * \param[in] he \b #HELEMENT, element which child elements you need to count
 * \param[out] count \b UINT*, variable to receive number of child elements
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * \par Example:
 * for paragraph defined as
 * \verbatim <p>Hello <b>wonderfull</b> world!</p> \endverbatim
 * count will be set to 1 as the paragraph has only one sub element:
 * \verbatim <b>wonderfull</b> \endverbatim
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetChildrenCount(HELEMENT he, UINT* count);

/**Get handle of every element's child element.
 * \param[in] he \b #HELEMENT
 * \param[in] n \b UINT, number of the child element
 * \param[out] phe \b #HELEMENT*, variable to receive handle of the child
 * element
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * \par Example:
 * for paragraph defined as
 * \verbatim <p>Hello <b>wonderfull</b> world!</p> \endverbatim
 * *phe will be equal to handle of &lt;b&gt; element:
 * \verbatim <b>wonderfull</b> \endverbatim
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetNthChild(HELEMENT he, UINT n, HELEMENT* phe);

/**Get parent element.
 * \param[in] he \b #HELEMENT, element which parent you need
 * \param[out] p_parent_he \b #HELEMENT*, variable to recieve handle of the
 * parent element
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetParentElement(HELEMENT he, HELEMENT* p_parent_he);

/** Get html representation of the element.
 * \param[in] he \b #HELEMENT
 * \param[out] utf8bytes \b pointer to byte address receiving UTF8 encoded HTML
 * \param[in] outer \b BOOL, if TRUE will retunr outer HTML otherwise inner.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 * OBSOLETE: use SciterGetElementHtmlCB instead
 */
//OBSOLETE EXTERN_C SCDOM_RESULT SCAPI SciterGetElementHtml(HELEMENT he, LPCBYTE* utf8bytes, BOOL outer);

/**Callback function used with #SciterGetElementHtmlCB().*/
typedef VOID SC_CALLBACK LPCBYTE_RECEIVER( LPCBYTE bytes, UINT num_bytes, LPVOID param );

/** Get html representation of the element.
 * \param[in] he \b #HELEMENT
 * \param[in] outer \b BOOL, if TRUE will retunr outer HTML otherwise inner.
 * \param[in] rcv \b pointer to function receiving UTF8 encoded HTML.
 * \param[in] rcv_param \b parameter that passed to rcv as it is.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 */
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementHtmlCB(HELEMENT he, BOOL outer, LPCBYTE_RECEIVER* rcv, LPVOID rcv_param);

 /**Get inner text of the element as LPWSTR (utf16 words).
 * \param[in] he \b #HELEMENT
 * \param[out] utf16words \b pointer to byte address receiving UTF16 encoded plain text
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 * OBSOLETE! use SciterGetElementTextCB instead
 */
 //OBSOLETE EXTERN_C SCDOM_RESULT SCAPI SciterGetElementText(HELEMENT he, LPWSTR* utf16);


 /**Callback function used with #SciterGetElementTextCB().*/
 typedef VOID SC_CALLBACK LPCWSTR_RECEIVER( LPCWSTR str, UINT str_length, LPVOID param );
 typedef VOID SC_CALLBACK LPCSTR_RECEIVER( LPCSTR str, UINT str_length, LPVOID param );

 /**Get inner text of the element as LPCWSTR (utf16 words).
 * \param[in] he \b #HELEMENT
 * \param[in] rcv \b pointer to the function receiving UTF16 encoded plain text
 * \param[in] rcv_param \b param passed that passed to LPCWSTR_RECEIVER "as is"
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 */

 EXTERN_C SCDOM_RESULT SCAPI SciterGetElementTextCB(HELEMENT he, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);


/**Set inner text of the element from LPCWSTR buffer (utf16 words).
 * \param[in] he \b #HELEMENT
 * \param[in] utf16words \b pointer, UTF16 encoded plain text
 * \param[in] length \b UINT, number of words in utf16words sequence
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 */
 EXTERN_C SCDOM_RESULT SCAPI SciterSetElementText(HELEMENT he, LPCWSTR utf16, UINT length);

/**Get number of element's attributes.
 * \param[in] he \b #HELEMENT
 * \param[out] p_count \b LPUINT, variable to receive number of element
 * attributes.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
 EXTERN_C SCDOM_RESULT SCAPI SciterGetAttributeCount(HELEMENT he, LPUINT p_count);

/**Get value of any element's attribute by attribute's number.
 * \param[in] he \b #HELEMENT
 * \param[in] n \b UINT, number of desired attribute
 * \param[out] p_name \b LPCSTR*, will be set to address of the string
 * containing attribute name
 * \param[out] p_value \b LPCWSTR*, will be set to address of the string
 * containing attribute value
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetNthAttributeNameCB(HELEMENT he, UINT n, LPCSTR_RECEIVER* rcv, LPVOID rcv_param);
EXTERN_C SCDOM_RESULT SCAPI SciterGetNthAttributeValueCB(HELEMENT he, UINT n, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);


/**Get value of any element's attribute by name.
 * \param[in] he \b #HELEMENT
 * \param[in] name \b LPCSTR, attribute name
 * \param[out] p_value \b LPCWSTR*, will be set to address of the string
 * containing attribute value
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
//EXTERN_C SCDOM_RESULT SCAPI SciterGetAttributeByName(HELEMENT he, LPCSTR name, LPCWSTR* p_value);
EXTERN_C SCDOM_RESULT SCAPI SciterGetAttributeByNameCB(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);

/**Set attribute's value.
 * \param[in] he \b #HELEMENT
 * \param[in] name \b LPCSTR, attribute name
 * \param[in] value \b LPCWSTR, new attribute value or 0 if you want to remove attribute.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetAttributeByName(HELEMENT he, LPCSTR name, LPCWSTR value);

/**Remove all attributes from the element.
 * \param[in] he \b #HELEMENT
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterClearAttributes(HELEMENT he);

/**Get element index.
 * \param[in] he \b #HELEMENT
 * \param[out] p_index \b LPUINT, variable to receive number of the element
 * among parent element's subelements.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementIndex(HELEMENT he, LPUINT p_index);

/**Get element's type.
 * \param[in] he \b #HELEMENT
 * \param[out] p_type \b LPCSTR*, receives name of the element type.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * \par Example:
 * For &lt;div&gt; tag p_type will be set to "div".
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementType(HELEMENT he, LPCSTR* p_type);

/**Get element's type.
 * \param[in] he \b #HELEMENT
 * \param[out] rcv \b LPCSTR_RECEIVER, receives name of the element type.
 * \param[out] rcv_param \b LPVOID, parameter passed as it is to the receiver.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementTypeCB(HELEMENT he, LPCSTR_RECEIVER* rcv, LPVOID rcv_param);

/**Get element's style attribute.
 * \param[in] he \b #HELEMENT
 * \param[in] name \b LPCSTR, name of the style attribute
 * \param[in] rcv \b pointer to the function receiving UTF16 encoded plain text
 * \param[in] rcv_param \b param passed that passed to LPCWSTR_RECEIVER "as is"
 *
 * Style attributes are those that are set using css. E.g. "font-face: arial" or "display: block".
 *
 * \sa #SciterSetStyleAttribute()
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetStyleAttributeCB(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);

/**Get element's style attribute.
 * \param[in] he \b #HELEMENT
 * \param[in] name \b LPCSTR, name of the style attribute
 * \param[out] value \b LPCWSTR, value of the style attribute.
 *
 * Style attributes are those that are set using css. E.g. "font-face: arial" or "display: block".
 *
 * \sa #SciterGetStyleAttribute()
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetStyleAttribute(HELEMENT he, LPCSTR name, LPCWSTR value);

/*Get bounding rectangle of the element.
 * \param[in] he \b #HELEMENT
 * \param[out] p_location \b LPRECT, receives bounding rectangle of the element
 * \param[in] rootRelative \b BOOL, if TRUE function returns location of the
 * element relative to Sciter window, otherwise the location is given
 * relative to first scrollable container.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/

enum ELEMENT_AREAS
{
  ROOT_RELATIVE = 0x01,       // - or this flag if you want to get HTMLayout window relative coordinates,
                              //   otherwise it will use nearest windowed container e.g. popup window.
  SELF_RELATIVE = 0x02,       // - "or" this flag if you want to get coordinates relative to the origin
                              //   of element iself.
  CONTAINER_RELATIVE = 0x03,  // - position inside immediate container.
  VIEW_RELATIVE = 0x04,       // - position relative to view - HTMLayout window

  CONTENT_BOX = 0x00,   // content (inner)  box
  PADDING_BOX = 0x10,   // content + paddings
  BORDER_BOX  = 0x20,   // content + paddings + border
  MARGIN_BOX  = 0x30,   // content + paddings + border + margins

  BACK_IMAGE_AREA = 0x40, // relative to content origin - location of background image (if it set no-repeat)
  FORE_IMAGE_AREA = 0x50, // relative to content origin - location of foreground image (if it set no-repeat)

  SCROLLABLE_AREA = 0x60,   // scroll_area - scrollable area in content box

};

EXTERN_C SCDOM_RESULT SCAPI SciterGetElementLocation(HELEMENT he, LPRECT p_location, UINT areas /*ELEMENT_AREAS*/);

enum SCITER_SCROLL_FLAGS
{
  SCROLL_TO_TOP = 0x01,
  SCROLL_SMOOTH = 0x10,
};

/*Scroll to view.
 * \param[in] he \b #HELEMENT
 * \param[in] SciterScrollFlags \b #UINT, combination of SCITER_SCROLL_FLAGS above or 0
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterScrollToView(HELEMENT he, UINT SciterScrollFlags);


/**Apply changes and refresh element area in its window.
 * \param[in] he \b #HELEMENT
 * \param[in] andForceRender \b BOOL, TRUE to force UpdateWindow() call.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterUpdateElement(HELEMENT he, BOOL andForceRender);

/**refresh element area in its window.
 * \param[in] he \b #HELEMENT
 * \param[in] rc \b RECT, rect to refresh.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterRefreshElementArea(HELEMENT he, RECT rc);

/**Set the mouse capture to the specified element.
 * \param[in] he \b #HELEMENT
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * After call to this function all mouse events will be targeted to the element.
 * To remove mouse capture call ReleaseCapture() function.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetCapture(HELEMENT he);
EXTERN_C SCDOM_RESULT SCAPI SciterReleaseCapture(HELEMENT he);

/**Get HWINDOW of containing window.
 * \param[in] he \b #HELEMENT
 * \param[out] p_hwnd \b HWINDOW*, variable to receive window handle
 * \param[in] rootWindow \b BOOL, handle of which window to get:
 * - TRUE - Sciter window
 * - FALSE - nearest parent element having overflow:auto or :scroll
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementHwnd(HELEMENT he, HWINDOW* p_hwnd, BOOL rootWindow);



/**Combine given URL with URL of the document element belongs to.
 * \param[in] he \b #HELEMENT
 * \param[in, out] szUrlBuffer \b LPWSTR, at input this buffer contains
 * zero-terminated URL to be combined, after function call it contains
 * zero-terminated combined URL
 * \param[in] UrlBufferSize \b UINT, size of the buffer pointed by
 * \c szUrlBuffer
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * This function is used for resolving relative references.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterCombineURL(HELEMENT he, LPWSTR szUrlBuffer, UINT UrlBufferSize);

/**Callback function used with #SciterVisitElement().*/
typedef BOOL SC_CALLBACK SciterElementCallback( HELEMENT he, LPVOID param );

/**Call specified function for every element in a DOM that meets specified
 * CSS selectors.
 * See list of supported selectors: http://terrainformatica.com/htmlayout/selectors.whtm
 * \param[in] he \b #HELEMENT
 * \param[in] selector \b LPCSTR, comma separated list of CSS selectors, e.g.: div, #id, div[align="right"].
 * \param[in] callback \b #SciterElementCallback*, address of callback
 * function being called on each element found.
 * \param[in] param \b LPVOID, additional parameter to be passed to callback
 * function.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSelectElements(
          HELEMENT  he,
          LPCSTR    CSS_selectors,
          SciterElementCallback*
                    callback,
          LPVOID    param);

EXTERN_C SCDOM_RESULT SCAPI SciterSelectElementsW(
          HELEMENT  he,
          LPCWSTR   CSS_selectors,
          SciterElementCallback*
                    callback,
          LPVOID    param);


/**Find parent of the element by CSS selector.
 * ATTN: function will test first element itself.
 * See list of supported selectors: http://terrainformatica.com/htmlayout/selectors.whtm
 * \param[in] he \b #HELEMENT
 * \param[in] selector \b LPCSTR, comma separated list of CSS selectors, e.g.: div, #id, div[align="right"].
 * \param[out] heFound \b #HELEMENT*, address of result HELEMENT
 * \param[in] depth \b LPVOID, depth of search, if depth == 1 then it will test only element itself.
 *                     Use depth = 1 if you just want to test he element for matching given CSS selector(s).
 *                     depth = 0 will scan the whole child parent chain up to the root.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSelectParent(
          HELEMENT  he,
          LPCSTR    selector,
          UINT      depth,
          /*out*/ HELEMENT* heFound);

EXTERN_C SCDOM_RESULT SCAPI SciterSelectParentW(
          HELEMENT  he,
          LPCWSTR   selector,
          UINT      depth,
          /*out*/ HELEMENT* heFound);


enum SET_ELEMENT_HTML
{
  SIH_REPLACE_CONTENT     = 0,
  SIH_INSERT_AT_START     = 1,
  SIH_APPEND_AFTER_LAST   = 2,
  SOH_REPLACE             = 3,
  SOH_INSERT_BEFORE       = 4,
  SOH_INSERT_AFTER        = 5
};

/**Set inner or outer html of the element.
 * \param[in] he \b #HELEMENT
 * \param[in] html \b LPCBYTE, UTF-8 encoded string containing html text
 * \param[in] htmlLength \b UINT, length in bytes of \c html.
 * \param[in] where \b UINT, possible values are:
 * - SIH_REPLACE_CONTENT - replace content of the element
 * - SIH_INSERT_AT_START - insert html before first child of the element
 * - SIH_APPEND_AFTER_LAST - insert html after last child of the element
 *
 * - SOH_REPLACE - replace element by html, a.k.a. element.outerHtml = "something"
 * - SOH_INSERT_BEFORE - insert html before the element
 * - SOH_INSERT_AFTER - insert html after the element
 *   ATTN: SOH_*** operations do not work for inline elements like <SPAN>
 *
 * \return /b #EXTERN_C SCDOM_RESULT SCAPI
  **/

EXTERN_C SCDOM_RESULT SCAPI SciterSetElementHtml(HELEMENT he, const BYTE* html, UINT htmlLength, UINT where);


/** Element UID support functions.
 *
 *  Element UID is unique identifier of the DOM element.
 *  UID is suitable for storing it in structures associated with the view/document.
 *  Access to the element using HELEMENT is more effective but table space of handles is limited.
 *  It is not recommended to store HELEMENT handles between function calls.
 **/


/** Get Element UID.
 * \param[in] he \b #HELEMENT
 * \param[out] puid \b UINT*, variable to receive UID of the element.
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * This function retrieves element UID by its handle.
 *
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetElementUID(HELEMENT he, UINT* puid);

/** Get Element handle by its UID.
 * \param[in] hwnd \b HWINDOW, HWINDOW of Sciter window
 * \param[in] uid \b UINT
 * \param[out] phe \b #HELEMENT*, variable to receive HELEMENT handle
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 * This function retrieves element UID by its handle.
 *
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetElementByUID(HWINDOW hwnd, UINT uid, HELEMENT* phe);

/** Shows block element (DIV) in popup window.
 * \param[in] hePopup \b HELEMENT, element to show as popup
 * \param[in] heAnchor \b HELEMENT, anchor element - hePopup will be shown near this element
 * \param[in] placement \b UINT, values:
 *     2 - popup element below of anchor
 *     8 - popup element above of anchor
 *     4 - popup element on left side of anchor
 *     6 - popup element on right side of anchor
 *     ( see numpad on keyboard to get an idea of the numbers)
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 *
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterShowPopup(HELEMENT hePopup, HELEMENT heAnchor, UINT placement);


/** Shows block element (DIV) in popup window at given position.
 * \param[in] hePopup \b HELEMENT, element to show as popup
 * \param[in] pos \b POINT, popup element position, relative to origin of Sciter window.
 * \param[in] animate \b BOOL, true if animation is needed.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterShowPopupAt(HELEMENT hePopup, POINT pos, BOOL animate);

/** Removes popup window.
 * \param[in] he \b HELEMENT, element which belongs to popup window or popup element itself
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterHidePopup(HELEMENT he);


/**Element callback function for all types of events. Similar to WndProc
 * \param tag \b LPVOID, tag assigned by SciterAttachElementProc function (like GWL_USERDATA)
 * \param he \b HELEMENT, this element handle (like HWINDOW)
 * \param evtg \b UINT, group identifier of the event, value is one of EVENT_GROUPS
 * \param prms \b LPVOID, pointer to group specific parameters structure.
 * \return TRUE if event was handled, FALSE otherwise.
 **/

typedef BOOL SC_CALLBACK ElementEventProc(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
typedef ElementEventProc* LPELEMENT_EVENT_PROC;

enum ELEMENT_STATE_BITS
{
   STATE_LINK             = 0x00000001,
   STATE_HOVER            = 0x00000002,
   STATE_ACTIVE           = 0x00000004,
   STATE_FOCUS            = 0x00000008,
   STATE_VISITED          = 0x00000010,
   STATE_CURRENT          = 0x00000020,  // current (hot) item
   STATE_CHECKED          = 0x00000040,  // element is checked (or selected)
   STATE_DISABLED         = 0x00000080,  // element is disabled
   STATE_READONLY         = 0x00000100,  // readonly input element
   STATE_EXPANDED         = 0x00000200,  // expanded state - nodes in tree view
   STATE_COLLAPSED        = 0x00000400,  // collapsed state - nodes in tree view - mutually exclusive with
   STATE_INCOMPLETE       = 0x00000800,  // one of fore/back images requested but not delivered
   STATE_ANIMATING        = 0x00001000,  // is animating currently
   STATE_FOCUSABLE        = 0x00002000,  // will accept focus
   STATE_ANCHOR           = 0x00004000,  // anchor in selection (used with current in selects)
   STATE_SYNTHETIC        = 0x00008000,  // this is a synthetic element - don't emit it's head/tail
   STATE_OWNS_POPUP       = 0x00010000,  // this is a synthetic element - don't emit it's head/tail
   STATE_TABFOCUS         = 0x00020000,  // focus gained by tab traversal
   STATE_EMPTY            = 0x00040000,  // empty - element is empty (text.size() == 0 && subs.size() == 0)
                                         //  if element has behavior attached then the behavior is responsible for the value of this flag.
   STATE_BUSY             = 0x00080000,  // busy; loading

   STATE_DRAG_OVER        = 0x00100000,  // drag over the block that can accept it (so is current drop target). Flag is set for the drop target block
   STATE_DROP_TARGET      = 0x00200000,  // active drop target.
   STATE_MOVING           = 0x00400000,  // dragging/moving - the flag is set for the moving block.
   STATE_COPYING          = 0x00800000,  // dragging/copying - the flag is set for the copying block.
   STATE_DRAG_SOURCE      = 0x01000000,  // element that is a drag source.
   STATE_DROP_MARKER      = 0x02000000,  // element is drop marker

   STATE_PRESSED          = 0x04000000,  // pressed - close to active but has wider life span - e.g. in MOUSE_UP it
                                         //   is still on; so behavior can check it in MOUSE_UP to discover CLICK condition.
   STATE_POPUP            = 0x08000000,  // this element is out of flow - popup

   STATE_IS_LTR           = 0x10000000,  // the element or one of its containers has dir=ltr declared
   STATE_IS_RTL           = 0x20000000,  // the element or one of its containers has dir=rtl declared

};

  /** Get/set state bits, stateBits*** accept or'ed values above
   **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementState( HELEMENT he, UINT* pstateBits);

/**
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetElementState( HELEMENT he, UINT stateBitsToSet, UINT stateBitsToClear, BOOL updateView);

/** Create new element, the element is disconnected initially from the DOM.
    Element created with ref_count = 1 thus you \b must call Sciter_UnuseElement on returned handler.
 * \param[in] tagname \b LPCSTR, html tag of the element e.g. "div", "option", etc.
 * \param[in] textOrNull \b LPCWSTR, initial text of the element or NULL. text here is a plain text - method does no parsing.
 * \param[out ] phe \b #HELEMENT*, variable to receive handle of the element
  **/

EXTERN_C SCDOM_RESULT SCAPI SciterCreateElement( LPCSTR tagname, LPCWSTR textOrNull, /*out*/ HELEMENT *phe );

/** Create new element as copy of existing element, new element is a full (deep) copy of the element and
    is disconnected initially from the DOM.
    Element created with ref_count = 1 thus you \b must call Sciter_UnuseElement on returned handler.
 * \param[in] he \b #HELEMENT, source element.
 * \param[out ] phe \b #HELEMENT*, variable to receive handle of the new element.
  **/
EXTERN_C SCDOM_RESULT SCAPI SciterCloneElement( HELEMENT he, /*out*/ HELEMENT *phe );

/** Insert element at \i index position of parent.
    It is not an error to insert element which already has parent - it will be disconnected first, but
    you need to update elements parent in this case.
 * \param index \b UINT, position of the element in parent collection.
   It is not an error to provide index greater than elements count in parent -
   it will be appended.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterInsertElement( HELEMENT he, HELEMENT hparent, UINT index );

/** Take element out of its container (and DOM tree).
    Element will be destroyed when its reference counter will become zero
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterDetachElement( HELEMENT he );

/** Take element out of its container (and DOM tree) and force destruction of all behaviors.
    Element will be destroyed when its reference counter will become zero
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterDeleteElement(HELEMENT he);

/** Start Timer for the element.
    Element will receive on_timer event
    To stop timer call SciterSetTimer( he, 0 );
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetTimer( HELEMENT he, UINT milliseconds, UINT_PTR timer_id );

/** Attach/Detach ElementEventProc to the element
    See htmlayout::event_handler.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterDetachEventHandler( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag );
/** Attach ElementEventProc to the element and subscribe it to events providede by subscription parameter
    See Sciter::attach_event_handler.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterAttachEventHandler( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag );


/** Attach/Detach ElementEventProc to the Sciter window.
    All events will start first here (in SINKING phase) and if not consumed will end up here.
    You can install Window EventHandler only once - it will survive all document reloads.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterWindowAttachEventHandler( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag, UINT subscription );
EXTERN_C SCDOM_RESULT SCAPI SciterWindowDetachEventHandler( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag );


/** SendEvent - sends sinking/bubbling event to the child/parent chain of he element.
    First event will be send in SINKING mode (with SINKING flag) - from root to he element itself.
    Then from he element to its root on parents chain without SINKING flag (bubbling phase).

 * \param[in] he \b HELEMENT, element to send this event to.
 * \param[in] appEventCode \b UINT, event ID, see: #BEHAVIOR_EVENTS
 * \param[in] heSource \b HELEMENT, optional handle of the source element, e.g. some list item
 * \param[in] reason \b UINT, notification specific event reason code
 * \param[out] handled \b BOOL*, variable to receive TRUE if any handler handled it, FALSE otherwise.

 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSendEvent(
          HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT reason, /*out*/ BOOL* handled);

/** PostEvent - post sinking/bubbling event to the child/parent chain of he element.
 *  Function will return immediately posting event into input queue of the application.
 *
 * \param[in] he \b HELEMENT, element to send this event to.
 * \param[in] appEventCode \b UINT, event ID, see: #BEHAVIOR_EVENTS
 * \param[in] heSource \b HELEMENT, optional handle of the source element, e.g. some list item
 * \param[in] reason \b UINT, notification specific event reason code

 **/

EXTERN_C SCDOM_RESULT SCAPI SciterPostEvent( HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT reason);


/** SciterCallMethod - calls behavior specific method.
 * \param[in] he \b HELEMENT, element - source of the event.
 * \param[in] params \b METHOD_PARAMS, pointer to method param block
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterCallBehaviorMethod(HELEMENT he, struct METHOD_PARAMS* params);

/** SciterRequestElementData  - request data download for the element.
 * \param[in] he \b HELEMENT, element to deleiver data to.
 * \param[in] url \b LPCWSTR, url to download data from.
 * \param[in] dataType \b UINT, data type, see SciterResourceType.
 * \param[in] hInitiator \b HELEMENT, element - initiator, can be NULL.

  event handler on the he element (if any) will be notified
  when data will be ready by receiving HANDLE_DATA_DELIVERY event.

  **/

EXTERN_C SCDOM_RESULT SCAPI SciterRequestElementData(
          HELEMENT he, LPCWSTR url, UINT dataType, HELEMENT initiator );

/**
 *  SciterSendRequest - send GET or POST request for the element
 *
 * event handler on the 'he' element (if any) will be notified
 * when data will be ready by receiving HANDLE_DATA_DELIVERY event.
 *
 **/

enum REQUEST_TYPE
{
  GET_ASYNC,  // async GET
  POST_ASYNC, // async POST
  GET_SYNC,   // synchronous GET
  POST_SYNC   // synchronous POST
};

//struct REQUEST_PARAM { LPCWSTR name; LPCWSTR value; };

EXTERN_C SCDOM_RESULT SCAPI SciterHttpRequest(
          HELEMENT        he,           // element to deliver data
          LPCWSTR         url,          // url
          UINT            dataType,     // data type, see SciterResourceType.
          UINT            requestType,  // one of REQUEST_TYPE values
          struct REQUEST_PARAM*  requestParams,// parameters
          UINT            nParams       // number of parameters
          );

/** SciterGetScrollInfo  - get scroll info of element with overflow:scroll or auto.
 * \param[in] he \b HELEMENT, element.
 * \param[out] scrollPos \b LPPOINT, scroll position.
 * \param[out] viewRect \b LPRECT, position of element scrollable area, content box minus scrollbars.
 * \param[out] contentSize \b LPSIZE, size of scrollable element content.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetScrollInfo(
         HELEMENT he, LPPOINT scrollPos, LPRECT viewRect, LPSIZE contentSize );

/** SciterSetScrollPos  - set scroll position of element with overflow:scroll or auto.
 * \param[in] he \b HELEMENT, element.
 * \param[in] scrollPos \b POINT, new scroll position.
 * \param[in] smooth \b BOOL, TRUE - do smooth scroll.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSetScrollPos(
         HELEMENT he, POINT scrollPos, BOOL smooth );

/** SciterGetElementIntrinsicWidths  - get min-intrinsic and max-intrinsic widths of the element.
 * \param[in] he \b HELEMENT, element.
 * \param[out] pMinWidth \b INT*, calculated min-intrinsic width.
 * \param[out] pMaxWidth \b INT*, calculated max-intrinsic width.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementIntrinsicWidths( HELEMENT he, INT* pMinWidth, INT* pMaxWidth );

/** SciterGetElementIntrinsicHeight  - get min-intrinsic height of the element calculated for forWidth.
 * \param[in] he \b HELEMENT, element.
 * \param[in] forWidth \b INT*, width to calculate intrinsic height for.
 * \param[out] pHeight \b INT*, calculated min-intrinsic height.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetElementIntrinsicHeight( HELEMENT he, INT forWidth, INT* pHeight );

/** SciterIsElementVisible - deep visibility, determines if element visible - has no visiblity:hidden and no display:none defined
    for itself or for any its parents.
 * \param[in] he \b HELEMENT, element.
 * \param[out] pVisible \b LPBOOL, visibility state.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterIsElementVisible( HELEMENT he, BOOL* pVisible);

/** SciterIsElementEnabled - deep enable state, determines if element enabled - is not disabled by itself or no one
    of its parents is disabled.
 * \param[in] he \b HELEMENT, element.
 * \param[out] pEnabled \b LPBOOL, enabled state.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterIsElementEnabled( HELEMENT he, BOOL* pEnabled );

/**Callback comparator function used with #SciterSortElements().
   Shall return -1,0,+1 values to indicate result of comparison of two elements
 **/
typedef INT SC_CALLBACK ELEMENT_COMPARATOR( HELEMENT he1, HELEMENT he2, LPVOID param );

/** SciterSortElements - sort children of the element.
 * \param[in] he \b HELEMENT, element which children to be sorted.
 * \param[in] firstIndex \b UINT, first child index to start sorting from.
 * \param[in] lastIndex \b UINT, last index of the sorting range, element with this index will not be included in the sorting.
 * \param[in] cmpFunc \b ELEMENT_COMPARATOR, comparator function.
 * \param[in] cmpFuncParam \b LPVOID, parameter to be passed in comparator function.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSortElements(
         HELEMENT he, UINT firstIndex, UINT lastIndex,
         ELEMENT_COMPARATOR* cmpFunc, LPVOID cmpFuncParam );

/** SciterSwapElements - swap element positions.
 * Function changes "insertion points" of two elements. So it swops indexes and parents of two elements.
 * \param[in] he1 \b HELEMENT, first element.
 * \param[in] he2 \b HELEMENT, second element.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterSwapElements(
         HELEMENT he1, HELEMENT he2 );


/** SciterTraverseUIEvent - traverse (sink-and-bubble) MOUSE or KEY event.
 * \param[in] evt \b EVENT_GROUPS, either HANDLE_MOUSE or HANDLE_KEY code.
 * \param[in] eventCtlStruct \b LPVOID, pointer on either MOUSE_PARAMS or KEY_PARAMS structure.
 * \param[out] bOutProcessed \b LPBOOL, pointer to BOOL receiving TRUE if event was processed by some element and FALSE otherwise.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterTraverseUIEvent(
         UINT evt, LPVOID eventCtlStruct, BOOL* bOutProcessed );

/** CallScriptingMethod - calls scripting method defined for the element.
 * \param[in] he \b HELEMENT, element which method will be callled.
 * \param[in] name \b LPCSTR, name of the method to call.
 * \param[in] argv \b SCITER_VALUE[], vector of arguments.
 * \param[in] argc \b UINT, number of arguments.
 * \param[out] retval \b SCITER_VALUE*, pointer to SCITER_VALUE receiving returning value of the function.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterCallScriptingMethod( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval );

/** CallScriptingFunction - calls scripting function defined in the namespace of the element (a.k.a. global function).
 * \param[in] he \b HELEMENT, element which namespace will be used.
 * \param[in] name \b LPCSTR, name of the method to call.
 * \param[in] argv \b SCITER_VALUE[], vector of arguments.
 * \param[in] argc \b UINT, number of arguments.
 * \param[out] retval \b SCITER_VALUE*, pointer to SCITER_VALUE receiving returning value of the function.
 *
 * SciterCallScriptingFunction allows to call functions defined on global level of main document or frame loaded in it.
 *
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterCallScriptingFunction( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval );

EXTERN_C SCDOM_RESULT SCAPI SciterEvalElementScript( HELEMENT he, LPCWSTR script, UINT scriptLength, VALUE* retval );

/**Attach HWINDOW to the element.
 * \param[in] he \b #HELEMENT
 * \param[in] hwnd \b HWINDOW, window handle to attach
 * \return \b #EXTERN_C SCDOM_RESULT SCAPI
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterAttachHwndToElement(HELEMENT he, HWINDOW hwnd);

/** Control types.
 *  Control here is any dom element having appropriate behavior applied
 **/

enum CTL_TYPE
{
    CTL_NO,               ///< This dom element has no behavior at all.
    CTL_UNKNOWN = 1,      ///< This dom element has behavior but its type is unknown.
    CTL_EDIT,             ///< Single line edit box.
    CTL_NUMERIC,          ///< Numeric input with optional spin buttons.
    CTL_CLICKABLE,        ///< toolbar button, behavior:clickable.
    CTL_BUTTON,           ///< Command button.
    CTL_CHECKBOX,         ///< CheckBox (button).
    CTL_RADIO,            ///< OptionBox (button).
    CTL_SELECT_SINGLE,    ///< Single select, ListBox or TreeView.
    CTL_SELECT_MULTIPLE,  ///< Multiselectable select, ListBox or TreeView.
    CTL_DD_SELECT,        ///< Dropdown single select.
    CTL_TEXTAREA,         ///< Multiline TextBox.
    CTL_HTMLAREA,         ///< WYSIWYG HTML editor.
    CTL_PASSWORD,         ///< Password input element.
    CTL_PROGRESS,         ///< Progress element.
    CTL_SLIDER,           ///< Slider input element.
    CTL_DECIMAL,          ///< Decimal number input element.
    CTL_CURRENCY,         ///< Currency input element.
    CTL_SCROLLBAR,

    CTL_HYPERLINK,

    CTL_MENUBAR,
    CTL_MENU,
    CTL_MENUBUTTON,

    CTL_CALENDAR,
    CTL_DATE,
    CTL_TIME,

    CTL_FRAME,
    CTL_FRAMESET,

    CTL_GRAPHICS,
    CTL_SPRITE,

    CTL_LIST,
    CTL_RICHTEXT,
    CTL_TOOLTIP,

    CTL_HIDDEN,
    CTL_URL,            ///< URL input element.
    CTL_TOOLBAR,
    
    CTL_FORM,
    CTL_FILE,           ///< file input element.
    CTL_PATH,           ///< path input element.
    CTL_WINDOW,         ///< has HWND attached to it

    CTL_LABEL,            
    CTL_IMAGE,            ///< image/object.  

};

/** SciterControlGetType - get type of control - type of behavior assigned to the element.
 * \param[in] he \b HELEMENT, element.
 * \param[out] pType \b UINT*, pointer to variable receiving control type,
 *             for list of values see CTL_TYPE.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterControlGetType( HELEMENT he, /*CTL_TYPE*/ UINT *pType );


/** SciterGetValue - get value of the element. 'value' is value of correspondent behavior attached to the element or its text.
 * \param[in] he \b HELEMENT, element which value will be retrieved.
 * \param[out] pval \b VALUE*, pointer to VALUE that will get elements value.
 *  ATTN: if you are not using json::value wrapper then you shall call ValueClear aginst the returned value
 *        otherwise memory will leak.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetValue( HELEMENT he, VALUE* pval );

/** SciterSetValue - set value of the element.
 * \param[in] he \b HELEMENT, element which value will be changed.
 * \param[in] pval \b VALUE*, pointer to the VALUE to set.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterSetValue( HELEMENT he, const VALUE* pval );

/** SciterGetExpando - get 'expando' of the element. 'expando' is a scripting object (of class Element)
 *  that is assigned to the DOM element. 'expando' could be null as they are created on demand by script.
 * \param[in] he \b HELEMENT, element which expando will be retrieved.
 * \param[out] pval \b VALUE*, pointer to VALUE that will get value of type T_OBJECT/UT_OBJECT_NATIVE or null.
 * \param[in] forceCreation \b BOOL, if there is no expando then when forceCreation==TRUE the function will create it.
 *  ATTN: if you are not using json::value wrapper then you shall call ValueClear aginst the returned value
 *        otherwise it will leak memory.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetExpando( HELEMENT he, VALUE* pval, BOOL forceCreation );

/** SciterGetObject - get 'expando' object of the element. 'expando' is a scripting object (of class Element)
 *  that is assigned to the DOM element. 'expando' could be null as they are created on demand by script.
 * \param[in] he \b HELEMENT, element which expando will be retrieved.
 * \param[out] pval \b tiscript::value*, pointer to tiscript::value that will get reference to the scripting object associated wuth the element or null.
 * \param[in] forceCreation \b BOOL, if there is no expando then when forceCreation==TRUE the function will create it.
 *
 *  ATTN!: if you plan to store the reference or use it inside code that calls script VM functions
 *         then you should use tiscript::pinned holder for the value.
 **/
EXTERN_C SCDOM_RESULT SCAPI SciterGetObject( HELEMENT he, tiscript_value* pval, BOOL forceCreation );

/** SciterGetElementNamespace - get namespace of document of the DOM element.
 * \param[in] he \b HELEMENT, element which expando will be retrieved.
 * \param[out] pval \b tiscript::value*, pointer to tiscript::value that will get reference to the namespace scripting object.
 *
 *  ATTN!: if you plan to store the reference or use it inside code that calls script VM functions
 *         then you should use tiscript::pinned holder for the value.
 **/

EXTERN_C SCDOM_RESULT SCAPI SciterGetElementNamespace(  HELEMENT he, tiscript_value* pval);

// get/set highlighted element. Used for debugging purposes.
EXTERN_C SCDOM_RESULT SCAPI SciterGetHighlightedElement(HWINDOW hwnd, HELEMENT* phe);
EXTERN_C SCDOM_RESULT SCAPI SciterSetHighlightedElement(HWINDOW hwnd, HELEMENT he);

//|
//| Nodes API
//|

// ATTENTION: node handles returned by functions below are not AddRef'ed

EXTERN_C SCDOM_RESULT SCAPI SciterNodeAddRef(HNODE hn);
EXTERN_C SCDOM_RESULT SCAPI SciterNodeRelease(HNODE hn);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeCastFromElement(HELEMENT he, HNODE* phn);
EXTERN_C SCDOM_RESULT SCAPI SciterNodeCastToElement(HNODE hn, HELEMENT* he);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeFirstChild(HNODE hn, HNODE* phn);
EXTERN_C SCDOM_RESULT SCAPI SciterNodeLastChild(HNODE hn, HNODE* phn);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeNextSibling(HNODE hn, HNODE* phn);
EXTERN_C SCDOM_RESULT SCAPI SciterNodePrevSibling(HNODE hn, HNODE* phn);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeParent(HNODE hnode, HELEMENT* pheParent);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeNthChild(HNODE hnode, UINT n, HNODE* phn);
EXTERN_C SCDOM_RESULT SCAPI SciterNodeChildrenCount(HNODE hnode, UINT* pn);

enum NODE_TYPE
{
  NT_ELEMENT,
  NT_TEXT,
  NT_COMMENT
};

EXTERN_C SCDOM_RESULT SCAPI SciterNodeType(HNODE hnode, UINT* pNodeType /*NODE_TYPE*/);

EXTERN_C SCDOM_RESULT SCAPI SciterNodeGetText(HNODE hnode, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
EXTERN_C SCDOM_RESULT SCAPI SciterNodeSetText(HNODE hnode, LPCWSTR text, UINT textLength);

enum NODE_INS_TARGET
{
  NIT_BEFORE,
  NIT_AFTER,
  NIT_APPEND,
  NIT_PREPEND,
};

EXTERN_C SCDOM_RESULT SCAPI SciterNodeInsert(HNODE hnode, UINT where /*NODE_INS_TARGET*/, HNODE what);
// remove the node from the DOM, use finalize=FALSE if you plan to attach the node to the DOM later.
EXTERN_C SCDOM_RESULT SCAPI SciterNodeRemove(HNODE hnode, BOOL finalize);

// ATTENTION: node handles returned by these two functions are AddRef'ed
EXTERN_C SCDOM_RESULT SCAPI SciterCreateTextNode(LPCWSTR text, UINT textLength, HNODE* phnode);
EXTERN_C SCDOM_RESULT SCAPI SciterCreateCommentNode(LPCWSTR text, UINT textLength, HNODE* phnode);

#include "sciter-x-behavior.h"

#endif
