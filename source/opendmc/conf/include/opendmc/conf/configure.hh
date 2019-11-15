/**************************************************************************//**
 * @file	configure.hh
 * @brief	配置編譯環境
 * @date	2000-10-10
 * @date	2019-11-12
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CONF_CONFIGURE_HH
#define	ODMC_CONF_CONFIGURE_HH

/**
 *	OpenDMC 設定項目
 */
#define ODMC_USES_DEBUGCONSOLE
#define ODMC_USES_XPSTYLE

/**
 *	配置 C++ 編譯器識別符
 *	Set C++ symbol
 */
#if defined(__cplusplus)
#	ifdef	ODMC_CPP
#	undef	ODMC_CPP
#	endif
#	define	ODMC_CPP
#endif

/**
 *	使用編譯環境檢測
 *	Check compiler for 64-bits mechanical
 */
#if defined(__x86_64__) || defined(__ppc64__) || defined(_M_X64) || defined(_M_AMD64)
#	ifndef	ODMC_IX64
#	define	ODMC_IX64
#	endif
#endif

#if defined(_M_IA64) || defined(_M_IX64)
#	ifndef	ODMC_IA64
#	define	ODMC_IA64
#	endif
#endif

#if defined(_M_IX86)
#	ifndef	ODMC_IX86
#	define	ODMC_IX86
#	endif
#endif

/**
 *	ODMC_UNICODE
 *	使用 UNICODE 識別符
 *	Set Unicode symbol
 */
#if defined(_UNICODE) || defined(UNICODE)
#	ifndef	ODMC_UNICODE
#	define	ODMC_UNICODE
#	endif
#endif

/**
 *	ODMC_DEBUG
 *	編譯環境除錯識別符
 *	Set debug symbol
 */
#if defined(_DEBUG) || defined(DEBUG)
#	ifndef	ODMC_DEBUG
#	define	ODMC_DEBUG
#	endif
#endif

/** Check & Set MSVC compiler symbol */
#if !defined(ODMC_CONF_WINDOWS_HH)
#include "windows.hh"
#endif

/** Check & define opendmc type */
#if !defined(ODMC_CONF_TYPEDEF_HH)
#include "typedef.hh"
#endif

/** Check & define opendmc macros */
#if !defined(ODMC_CONF_MACROS_HH)
#include "macros.hh"
#endif

#endif // !ODMC_CONF_CONFIGURE_HH

