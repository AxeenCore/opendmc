/**************************************************************************//**
 * @file	windows.hh
 * @brief	Windows 編譯環境檢測
 * @date	2000-10-10
 * @date	2010-08-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CONF_WINDOWS_HH
#define	ODMC_CONF_WINDOWS_HH

 /**
  *	編譯器識別符 ODMC_MSVC
  *	Set MSVC compiler symbol
  *
  *	MS VC++ 16.0 _MSC_VER = 1920 (Visual Studio 2019)
  *	MS VC++ 15.0 _MSC_VER = 1910 (Visual Studio 2017)
  *	MS VC++ 14.0 _MSC_VER = 1900 (Visual Studio 2015)
  *	MS VC++ 12.0 _MSC_VER = 1800 (Visual Studio 2013)
  *	MS VC++ 11.0 _MSC_VER = 1700 (Visual Studio 2012)
  *	MS VC++ 10.0 _MSC_VER = 1600 (Visual Studio 2010)
  *	MS VC++  9.0 _MSC_VER = 1500 (Visual Studio 2008)
  *	MS VC++  8.0 _MSC_VER = 1400 (Visual Studio 2005)
  *	MS VC++  7.1 _MSC_VER = 1310 (Visual Studio 2003)
  *	MS VC++  7.0 _MSC_VER = 1300 (Visual Studio.NET)
  *	MS VC++  6.0 _MSC_VER = 1200 (Visual Studio 98)
  *	MS VC++  5.0 _MSC_VER = 1100 (Visual Studio 97)
  *
  *	若支援辦本低於指定要求，啟動編譯將發出錯誤訊息
  */
#if defined(_MSC_VER)
#	if _MSC_VER	< 1800
#		error "This Visual C++ version is not supported any longer (at least Visual Studio 2013 (MSVC v12) required)."
#	elif _MSC_VER < 1900
#		define ODMC_MSVC	12	// vs2013
#	elif _MSC_VER < 1910
#		define ODMC_MSVC	14	// vs2015
#	elif _MSC_VER < 1919
#		define ODMC_MSVC	15	// vs2017
#	elif _MSC_VER < 1930
#		define ODMC_MSVC	16	// vs2019
#	else
#		pragma message("Please update opendmc/conf/configure.hh to recognize this VC++ version")
#	endif
#endif

/**
 *	ODMC_WIN32
 *	Windows 編譯環境識別符
 *	Set windows compiler
 */
#if defined(_WIN32) || defined(WIN32)
#	ifndef	ODMC_WIN32
#	define	ODMC_WIN32
#	endif
#endif

/**
 *	ODMC_WIN64
 *	Windows 64bit 編譯環境識別符
 *	Set windows 64-bits compiler
 */
#if defined(_WIN64) || defined(WIN64)
#	ifndef	ODMC_WIN64
#	define	ODMC_WIN64
#	endif
#endif

/**
 *	ODMC_WINDOWS
 *	編譯環境識別符 - Windows 作業環境識別符。
 *	Set Microsoft Windows Symbol
 */
#if defined(ODMC_WIN32) || defined(ODMC_WIN64)
#	ifndef	ODMC_WINDOWS
#	define	ODMC_WINDOWS
#	endif
#endif

/**
 *	ODMC_WINCONSOLE
 *	編譯環境識別符 - Windows console
 *	Set MSVC console symbol
 */
#if defined(ODMC_MSVC) && defined(_CONSOLE)
#	ifndef	ODMC_WINCONSOLE
#	define	ODMC_WINCONSOLE
#	endif
#endif

/**
 *	ODMC_WINLIB
 *	編譯環境識別符 - Windows library
 *	Set MSVC library symbol
 */
#if defined(ODMC_MSVC) && defined(_LIB)
#	ifndef	ODMC_WINLIB
#	define	ODMC_WINLIB
#	endif
#endif

/**
 *	ODMC_WINDLL
 *	編譯環境識別符 - Windows library
 *	Set MSVC dynamic library symbol
 */
#if defined(ODMC_MSVC)
#	if defined(_WINDLL) || defined(_USRDLL)
#		ifndef ODMC_WINDLL
#		define ODMC_WINDLL
#		endif
#	endif
#endif

/**
 *	ODMC_WINVER
 *	指定編譯 Windows 版本支援最低要求
 *	Set Windows version minimum requirements
 */
#if defined(ODMC_WINDOWS)
#	ifndef	ODMC_WIN32_PLATFORM_SYMBOL
#		define	ODMC_WIN32_PLATFORM_SYMBOL
#		define	ODMC_WIN32_WINNT4		0x0400  //!< Windows NT 4.0
#		define	ODMC_WIN32_WIN2K		0x0500  //!< Windows 2000
#		define	ODMC_WIN32_WINXP		0x0501  //!< Windows XP
#		define	ODMC_WIN32_WIN2K3		0x0502  //!< Windows Server 2003
#		define	ODMC_WIN32_WINVISTA		0x0600  //!< Windows Vista
#		define	ODMC_WIN32_WIN2K8		0x0600  //!< Windows Server 2008
#		define	ODMC_WIN32_WIN7			0x0601  //!< Windows 7
#		define	ODMC_WIN32_WIN8			0x0602  //!< Windows 8
#		define	ODMC_WIN32_WIN8BLUE		0x0603  //!< Windows 8.1
#		define	ODMC_WIN32_WINTHRESHOLD	0x0A00	//!< ABRACADABRA_THRESHOLD
#		define	ODMC_WIN32_WIN10		0x0A00	//!< Windows 10	ABRACADABRA_THRESHOLD
#	endif
#
#	ifndef	ODMC_WINVER
#	define	ODMC_WINVER	ODMC_WIN32_WIN7
#	endif
#endif

/**
 *	ODMC_WINIEVER
 *	指定編譯 IE 版本支援最低要求
 *	Set IE version minimum requirements
 */
#if defined(ODMC_WINDOWS)
#	ifndef	ODMC_WIN32_IEVER
#		define ODMC_WIN32_IEVER
#		define ODMC_IE_IE20			0x0200	//!< IE 2.0
#		define ODMC_IE_IE30			0x0300	//!< IE 3.0
#		define ODMC_IE_IE302		0x0302	//!< IE 3.2
#		define ODMC_IE_IE40			0x0400	//!< IE 4.0
#		define ODMC_IE_IE401		0x0401	//!< IE 4.1
#		define ODMC_IE_IE50			0x0500	//!< IE 5.0
#		define ODMC_IE_IE501		0x0501	//!< IE 5.1
#		define ODMC_IE_IE55			0x0550	//!< IE 5.5
#		define ODMC_IE_IE60			0x0600	//!< IE 6.0
#		define ODMC_IE_IE60SP1		0x0601	//!< IE 6.0 SP1
#		define ODMC_IE_IE60SP2		0x0603	//!< IE 6.0 SP2
#		define ODMC_IE_IE70			0x0700	//!< IE 7.0
#		define ODMC_IE_IE80			0x0800	//!< IE 8.0
#		define ODMC_IE_IE90			0x0900	//!< IE 9.0
#		define ODMC_IE_IE100		0x0A00	//!< IE 10.0
#		define ODMC_IE_IE110		0x0A00	//!< IE 11.0 ABRACADABRA_THRESHOLD
#	endif
#
#	ifndef	ODMC_IEVER
#	define	ODMC_IEVER	ODMC_IE_IE110
#	endif
#endif

/**
 *	Windows 程式編譯版本識別
 *	Set compiler windows application minimum requirements
 */
#if defined(ODMC_WINDOWS)
#	/** WINVER */
#	ifndef WINVER
#	define WINVER ODMC_WINVER
#	else
#		if WINVER < ODMC_WINVER
#			undef  WINVER
#			define WINVER ODMC_WINVER
#		endif
#	endif
#
#	/** _WIN32_IE */
#	ifndef _WIN32_IE
#	define _WIN32_IE ODMC_IEVER
#	else
#		if _WIN32_IE < ODMC_IEVER
#			undef	_WIN32_IE
#			define	_WIN32_IE ODMC_IEVER
#		endif
#	endif
#endif

/**
 *	Windows 程式編譯, 功能設定
 */
#if defined(ODMC_WINDOWS)
#	/** To do speed up compilations, before include windows.h */
#	ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#	endif
#
#	/** Enable features that require more care in declaring and using types. */
#	ifndef STRICT
#	define STRICT
#	endif
#
#	/** Disable Compiler Warning(level 3 or high) C4996 -- disable unsafe deprecation. */
#	if defined(ODMC_MSVC)
#		ifndef	_CRT_SECURE_NO_WARNINGS
#		define	_CRT_SECURE_NO_WARNINGS
#		endif
#
#		ifndef	_CRT_NON_CONFORMING_SWPRINTFS
#		define	_CRT_NON_CONFORMING_SWPRINTFS
#		endif
#
#		ifndef	_SCL_SECURE_NO_WARNINGS
#		define	_SCL_SECURE_NO_WARNINGS
#		endif
#	endif
#
#	/** Used dllexport prefix */
#	ifdef DLLEXPORT
#	undef DLLEXPORT
#	endif
#
#	if defined(ODMC_WINDLL)
#		define DLLEXPORT __declspec(dllexport)
#		define DLLIMPORT __declspec(dllimport)
#	else
#		define DLLEXPORT
#		define DLLIMPORT
#	endif
#
#	/** 啟用 console debug 介面 */
#	if defined(ODMC_USES_DEBUGCONSOLE)
#		ifdef ODMC_UNICODE
#			pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")
#		else
#			pragma comment(linker, "/subsystem:console /entry:WinMainCRTStartup")
#		endif
#	endif
#
#	/** Windows GUI Application style */
#	if defined(ODMC_USES_XPSTYLE)
#	if !defined(ODMC_WINLIB) && !defined(ODMC_WINCONSOLE)
#		if defined _M_IX86
#			pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		elif defined _M_IA64
#			pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		elif defined _M_X64
#			pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		else
#			pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		endif
#	endif
#	endif
#endif

#endif // !ODMC_CONF_WINDOWS_HH


