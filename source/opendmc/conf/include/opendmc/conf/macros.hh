/**************************************************************************//**
 * @file	macros.hh
 * @brief	Opendmc 巨集定義
 * @date	2000-10-10
 * @date	2019-11-12
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CONF_MACROS
#define ODMC_CONF_MACROS

/**
 *	TEXT 立即型文字字串 巨集
 */
#if !defined(ODMC_WINDOWS)
#	ifdef ODMC_UNICODE
#		ifndef __TEXT
#		define __TEXT(quote) L ## quote		//!< 字串定義巨集
#		endif
#	else
#		ifndef __TEXT
#		define __TEXT(quote) quote			//!< 字串定義巨集
#		endif
#	endif
#
#	ifndef TEXT
#		define TEXT(quote) __TEXT(quote)	//!< 字串定義巨集
#	endif
#endif

/**
 *	物件、記憶體釋放
 */
#define SAFE_FREE(p)            { if (nullptr!=(p))  { ::free((void*)(p); (p)=nullptr; } }
#define SAFE_DELETE(p)          { if (nullptr!=(p))  { delete (p); (p)=nullptr; } }
#define SAFE_DELETEARRAY(p)		{ if (nullptr!=(p))  { delete [] (p); (p)=nullptr; } }
#define SAFE_CLOSE_FILE(p)      { if (nullptr!=(p))  { ::fclose(p); (p)=nullptr; } }
#define SAFE_CLOSE_HANDLE(p)    { if (nullptr!=(p) && INVALID_HANDLE_VALUE!=(p))  { ::CloseHandle(p); (p)=nullptr; } }

/**
 *	除錯相關聚集定義，Debug message macro
 */
#if defined(ODMC_UNICODE)
#	define __FILE_T__	_CRT_WIDE(__FILE__)
#	define __FUNC_T__	_CRT_WIDE(__FUNCTION__)
#	define ConsoleText	wprintf
#else
#	define __FILE_T__	__FILE__
#	define __FUNC_T__	__FUNCTION__
#	define ConsoleText	printf
#endif

/**
 *	buffer size macro
 */
#define MAX_STRING_PATH			260		//!< max path name string length
#define MAX_STRING_FILE			260		//!< max file name string length
#define MAX_STRING_WNDCLASS		1024	//!< max class name string length
#define MAX_STRING_CAPTION		1024	//!< max caption name string length
#define	MAX_STRING_MENU			80		//!< max menu item string length

#define BUFF_SIZE_LARGE			1024	//!< large buffer
#define BUFF_SIZE_MIDDLE		512		//!< middle buffer
#define BUFF_SIZE_SMALL			256		//!< small buffer
#define BUFF_SIZE_TINY			16		//!< tiny buffer

#endif // !ODMC_CONF_MACROS
