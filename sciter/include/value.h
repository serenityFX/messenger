#ifndef __value_h__
#define __value_h__

#include "sciter-x-types.h"


#ifdef WIN32
#include <windows.h>
#endif

enum VALUE_RESULT
{
  HV_OK_TRUE = -1,
  HV_OK = 0,
  HV_BAD_PARAMETER = 1,
  HV_INCOMPATIBLE_TYPE = 2
};

typedef struct
{
  UINT     t;
  UINT     u;
  UINT64   d;
} VALUE;

#define FLOAT_VALUE   double

struct NATIVE_FUNCTOR_VALUE;

enum VALUE_TYPE
{
    T_UNDEFINED = 0,
    T_NULL = 1,
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_DATE,     // INT64 - contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC), a.k.a. FILETIME on Windows
    T_CURRENCY, // INT64 - 14.4 fixed number. E.g. dollars = int64 / 10000; 
    T_LENGTH,   // length units, value is int or float, units are VALUE_UNIT_TYPE
    T_ARRAY,
    T_MAP,
    T_FUNCTION,
    T_BYTES,      // sequence of bytes - e.g. image data
    T_OBJECT,     // scripting object proxy (TISCRIPT/SCITER)
    T_DOM_OBJECT  // DOM object (CSSS!), use get_object_data to get HELEMENT 
};

enum VALUE_UNIT_TYPE
{
    UT_EM = 1, //height of the element's font. 
    UT_EX = 2, //height of letter 'x' 
    UT_PR = 3, //%
    UT_SP = 4, //%% "springs", a.k.a. flex units
    reserved1 = 5, 
    reserved2 = 6, 
    UT_PX = 7, //pixels
    UT_IN = 8, //inches (1 inch = 2.54 centimeters). 
    UT_CM = 9, //centimeters. 
    UT_MM = 10, //millimeters. 
    UT_PT = 11, //points (1 point = 1/72 inches). 
    UT_PC = 12, //picas (1 pica = 12 points). 
    UT_DIP = 13,
    reserved3 = 14, 
    UT_COLOR = 15, // color in int
    UT_URL   = 16,  // url in string
    UT_SYMBOL = 0xFFFF, // for T_STRINGs designates symbol string ( so called NAME_TOKEN - CSS or JS identifier )
};

enum VALUE_UNIT_TYPE_DATE
{
    DT_HAS_DATE         = 0x01, // date contains date portion
    DT_HAS_TIME         = 0x02, // date contains time portion HH:MM
    DT_HAS_SECONDS      = 0x04, // date contains time and seconds HH:MM:SS
    DT_UTC              = 0x10, // T_DATE is known to be UTC. Otherwise it is local date/time
};

// Sciter or TIScript specific
enum VALUE_UNIT_TYPE_OBJECT
{
    UT_OBJECT_ARRAY  = 0,   // type T_OBJECT of type Array
    UT_OBJECT_OBJECT = 1,   // type T_OBJECT of type Object
    UT_OBJECT_CLASS  = 2,   // type T_OBJECT of type Type (class or namespace)
    UT_OBJECT_NATIVE = 3,   // type T_OBJECT of native Type with data slot (LPVOID)
    UT_OBJECT_FUNCTION = 4, // type T_OBJECT of type Function
    UT_OBJECT_ERROR = 5,    // type T_OBJECT of type Error
};


/**
 * ValueInit - initialize VALUE storage
 * This call has to be made before passing VALUE* to any other functions
 */
EXTERN_C UINT SCAPI ValueInit( VALUE* pval );

/**
 * ValueClear - clears the VALUE and deallocates all assosiated structures that are not used anywhere else.
 */
EXTERN_C UINT SCAPI ValueClear( VALUE* pval );

/**
 * ValueCompare - compares two values, returns HV_OK_TRUE if val1 == val2.
 */
EXTERN_C UINT SCAPI ValueCompare( const VALUE* pval1, const VALUE* pval2 );

/**
 * ValueCopy - copies src VALUE to dst VALUE. dst VALUE must be in ValueInit state.
 */
EXTERN_C UINT SCAPI ValueCopy( VALUE* pdst, const VALUE* psrc );

/**
 * ValueIsolate - converts T_OBJECT value types to T_MAP or T_ARRAY.
 * use this method if you need to pass values between different threads.
 * The fanction is applicable for the Sciter 
 */
EXTERN_C UINT SCAPI ValueIsolate( VALUE* pdst );

/**
 * ValueType - returns VALUE_TYPE and VALUE_UNIT_TYPE flags of the VALUE
 */
EXTERN_C UINT SCAPI ValueType( const VALUE* pval, UINT* pType, UINT* pUnits );

/**
 * ValueStringData - returns string data for T_STRING type
 * For T_FUNCTION returns name of the fuction. 
 */
EXTERN_C UINT SCAPI ValueStringData( const VALUE* pval, LPCWSTR* pChars, UINT* pNumChars );

/**
 * ValueStringDataSet - sets VALUE to T_STRING type and copies chars/numChars to
 * internal refcounted buffer assosiated with the value. 
 */
EXTERN_C UINT SCAPI ValueStringDataSet( VALUE* pval, LPCWSTR chars, UINT numChars, UINT units );

/**
 * ValueIntData - retreive integer data of T_INT, T_LENGTH and T_BOOL types
 */
EXTERN_C UINT SCAPI ValueIntData( const VALUE* pval, INT* pData );

/**
 * ValueIntDataSet - sets VALUE integer data of T_INT and T_BOOL types 
 * Optionally sets units field too.
 */
EXTERN_C UINT SCAPI ValueIntDataSet( VALUE* pval, INT data, UINT type, UINT units );

/**
 * ValueInt64Data - retreive 64bit integer data of T_CURRENCY and T_DATE values.
 */
EXTERN_C UINT SCAPI ValueInt64Data( const VALUE* pval, INT64* pData );

/**
 * ValueInt64DataSet - sets 64bit integer data of T_CURRENCY and T_DATE values.
 */
EXTERN_C UINT SCAPI ValueInt64DataSet( VALUE* pval, INT64 data, UINT type, UINT units );

/**
 * ValueFloatData - retreive FLOAT_VALUE (double) data of T_FLOAT and T_LENGTH values.
 */
EXTERN_C UINT SCAPI ValueFloatData( const VALUE* pval, FLOAT_VALUE* pData );

/**
 * ValueFloatDataSet - sets FLOAT_VALUE data of T_FLOAT and T_LENGTH values.
 */
EXTERN_C UINT SCAPI ValueFloatDataSet( VALUE* pval, FLOAT_VALUE data, UINT type, UINT units );

/**
 * ValueBinaryData - retreive integer data of T_BYTES type
 */
EXTERN_C UINT SCAPI ValueBinaryData( const VALUE* pval, LPCBYTE* pBytes, UINT* pnBytes );

/**
 * ValueBinaryDataSet - sets VALUE to sequence of bytes of type T_BYTES 
 * 'type' here must be set to T_BYTES. Optionally sets units field too.
 * The function creates local copy of bytes in its own storage.
 */
EXTERN_C UINT SCAPI ValueBinaryDataSet( VALUE* pval, LPCBYTE pBytes, UINT nBytes, UINT type, UINT units );

/**
 * ValueElementsCount - retreive number of sub-elements for:
 * - T_ARRAY - number of elements in the array; 
 * - T_MAP - number of key/value pairs in the map;
 * - T_FUNCTION - number of arguments in the function;
 */
EXTERN_C UINT SCAPI ValueElementsCount( const VALUE* pval, INT* pn);

/**
 * ValueNthElementValue - retreive value of sub-element at index n for:
 * - T_ARRAY - nth element of the array; 
 * - T_MAP - value of nth key/value pair in the map;
 * - T_FUNCTION - value of nth argument of the function;
 */
EXTERN_C UINT SCAPI ValueNthElementValue( const VALUE* pval, INT n, VALUE* pretval);

/**
 * ValueNthElementValueSet - sets value of sub-element at index n for:
 * - T_ARRAY - nth element of the array; 
 * - T_MAP - value of nth key/value pair in the map;
 * - T_FUNCTION - value of nth argument of the function;
 * If the VALUE is not of one of types above then it makes it of type T_ARRAY with 
 * single element - 'val_to_set'.
 */
EXTERN_C UINT SCAPI ValueNthElementValueSet( VALUE* pval, INT n, const VALUE* pval_to_set);

/**Callback function used with #ValueEnumElements().
 * return TRUE to continue enumeration
 */
typedef BOOL SC_CALLBACK KeyValueCallback( LPVOID param, const VALUE* pkey, const VALUE* pval );

/**
 * ValueEnumElements - enumeartes key/value pairs of T_MAP, T_FUNCTION and T_OBJECT values
 * - T_MAP - key of nth key/value pair in the map;
 * - T_FUNCTION - name of nth argument of the function (if any);
 */
EXTERN_C UINT SCAPI ValueNthElementKey( const VALUE* pval, INT n, VALUE* pretval);

EXTERN_C UINT SCAPI ValueEnumElements( VALUE* pval, KeyValueCallback* penum, LPVOID param);

/**
 * ValueSetValueToKey - sets value of sub-element by key:
 * - T_MAP - value of key/value pair with the key;
 * - T_FUNCTION - value of argument with the name key;
 * - T_OBJECT (tiscript) - value of property of the object
 * If the VALUE is not of one of types above then it makes it of type T_MAP with 
 * single pair - 'key'/'val_to_set'.
 *
 * key usually is a value of type T_STRING
 *
 */
EXTERN_C UINT SCAPI ValueSetValueToKey( VALUE* pval, const VALUE* pkey, const VALUE* pval_to_set);

/**
 * ValueGetValueOfKey - retrieves value of sub-element by key:
 * - T_MAP - value of key/value pair with the key;
 * - T_FUNCTION - value of argument with the name key;
 * - T_OBJECT (tiscript) - value of property of the object
 * Otherwise *pretval will have T_UNDEFINED value.
 */
EXTERN_C UINT SCAPI ValueGetValueOfKey( const VALUE* pval, const VALUE* pkey, VALUE* pretval);

enum VALUE_STRING_CVT_TYPE
{
  CVT_SIMPLE,        ///< simple conversion of terminal values 
  CVT_JSON_LITERAL,  ///< json literal parsing/emission 
  CVT_JSON_MAP,      ///< json parsing/emission, it parses as if token '{' already recognized 
  CVT_XJSON_LITERAL, ///< x-json parsing/emission, date is emitted as ISO8601 date literal, currency is emitted in the form DDDD$CCC
                                                   
};

/**
 * ValueToString - converts value to T_STRING inplace:
 * - CVT_SIMPLE - parse/emit terminal values (T_INT, T_FLOAT, T_LENGTH, T_STRING)
 * - CVT_JSON_LITERAL - parse/emit value using JSON literal rules: {}, [], "string", true, false, null 
 * - CVT_JSON_MAP - parse/emit MAP value without enclosing '{' and '}' brackets.
 */
EXTERN_C UINT SCAPI ValueToString( VALUE* pval, /*VALUE_STRING_CVT_TYPE*/ UINT how );

/**
 * ValueFromString - parses string into value:
 * - CVT_SIMPLE - parse/emit terminal values (T_INT, T_FLOAT, T_LENGTH, T_STRING), "guess" non-strict parsing
 * - CVT_JSON_LITERAL - parse/emit value using JSON literal rules: {}, [], "string", true, false, null 
 * - CVT_JSON_MAP - parse/emit MAP value without enclosing '{' and '}' brackets.
 * Returns:
 *   Number of non-parsed characters in case of errors. Thus if string was parsed in full it returns 0 (success)  
 */
EXTERN_C UINT SCAPI ValueFromString( VALUE* pval, LPCWSTR str, UINT strLength, /*VALUE_STRING_CVT_TYPE*/ UINT how );

/**
 * ValueInvoke - function invocation (Sciter/TIScript).
 * - VALUE* pval is a value of type T_OBJECT/UT_OBJECT_FUNCTION
 * - VALUE* pthis - object that will be known as 'this' inside that function.
 * - UINT argc, const VALUE* argv - vector of arguments to pass to the function. 
 * - VALUE* pretval - parse/emit MAP value without enclosing '{' and '}' brackets.
 * - LPCWSTR url - url or name of the script - used for error reporting in the script.
 * Returns:
 *   HV_OK, HV_BAD_PARAMETER or HV_INCOMPATIBLE_TYPE
 */
EXTERN_C UINT SCAPI ValueInvoke( VALUE* pval, VALUE* pthis, UINT argc, const VALUE* argv, VALUE* pretval, LPCWSTR url);

/**
 * ValueNativeFunctorSet - set reference to native function
 * - VALUE* pval - value to be initialized 
 * - NATIVE_FUNCTOR_VALUE* pnfv - reference to native functor implementation.
 * Returns:
 *   HV_OK, HV_BAD_PARAMETER or HV_INCOMPATIBLE_TYPE
 */
EXTERN_C UINT SCAPI ValueNativeFunctorSet( VALUE* pval, struct NATIVE_FUNCTOR_VALUE* pnfv);

EXTERN_C BOOL SCAPI ValueIsNativeFunctor( const VALUE* pval);


#if defined(__cplusplus) && !defined(__value_hpp__)

  #include "value.hpp"

  #pragma warning( push )
  #pragma warning(disable:4786) //identifier was truncated...

  namespace json
  {
    using sciter::value;
    using sciter::string;
    using sciter::astring;
  }

#endif //defined(__cplusplus)

#endif
