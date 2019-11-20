/**************************************************************************//**
 * @file	typedef.hh
 * @brief	OpenDmc 型別定義
 * @date	2000-10-10
 * @date	2019-11-12
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CONF_TYPEDEF_HH
#define	ODMC_CONF_TYPEDEF_HH

/**
 * Include C CRT header
 */
#if !defined(ODMC_MSVC)
#	include <stddef.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <assert.h>

/**
 * Include C++ CRT header
 */
#if defined(ODMC_CPP)
#	include <iostream>
#	include <iomanip>
#	include <fstream>
#	include <string>
#	include <sstream>
#	include <vector>
#	include <map>
#endif

/**
 * Windows Win32API header
 */
#if defined(ODMC_WINDOWS)
#	include <windows.h>
#	include <tchar.h>
#	include <commctrl.h>
#	include <tlhelp32.h>
#	include <timeapi.h>
#	include <shellapi.h>
#	include "unfuncdef.hh"
#endif

/**
 *	OpenDmc 基礎型別定義
 */
#if !defined(ODMC_WINDOWS)
	typedef __int8				INT8,	SBYTE,	*PINT8;		//!< 8	位元, 整數型別 (帶正負號)
	typedef __int16				INT16,	SWORD,	*PINT16;	//!< 16	位元, 整數型別 (帶正負號)
	typedef __int32				INT32,	SDWORD	*PINT32;	//!< 32 位元, 整數型別 (帶正負號)
	typedef __int64				INT64,	SQWORD,	*PINT64;	//!< 64 位元, 整數型別 (帶正負號)
	typedef unsigned __int8		UINT8,	BYTE,	*PUINT8;	//!< 8	位元, 整數型別
	typedef unsigned __int16	UINT16, WORD,	*PUINT16;	//!< 16	位元, 整數型別
	typedef unsigned __int32	UINT32, DWORD,	*PUINT32;	//!< 32	位元, 整數型別
	typedef unsigned __int64	UINT64, QWORD,	*PUINT64;	//!< 64	位元, 整數型別
#	if !defined(ODMC_UNICODE)
	typedef char				TCHAR;				//!< 字元 - 單位元組
#	else
	typedef wchar_t				TCHAR;				//!< 字元 - 雙位元組
#	endif
#endif

/**
 *	數值類型最大型別定義
 */
#if defined(ODMC_IX64) || defined(ODMC_IA64)
	typedef INT64	QINT;		//!< 有號數，對應CPU編譯架構最大型別 64bit / 32bit
	typedef UINT64	QUINT;		//!< 無號數，對應CPU編譯架構最大型別 64bit / 32bit
	typedef	UINT64	ADDRS;		//!< 位址型別定義
#else
	typedef INT32	QINT;		//!< 有號數，對應CPU編譯架構最大型別 64bit / 32bit
	typedef UINT32	QUINT;		//!< 有號數，對應CPU編譯架構最大型別 64bit / 32bit
	typedef UINT32	ADDRS;		//!< 位址型別定義
#endif

/**
 * Define the calling convention symbol
 */
#ifndef STDCALL
#define STDCALL		__stdcall		//<! 由最後參數先開始推入堆疊 (x64 會利用暫存器加速，原理不變)
#endif

#ifndef PASCALL
#define	PASCALL		__stdcall		//<! 由最後參數先開始推入堆疊 (x64 會利用暫存器加速，原理不變)
#endif

#ifndef CDECALL
#define CDECALL		__cdecl			//!< 由最後參數先開始推入堆疊 (x64 會利用暫存器加速，原理不變)
#endif

#ifndef FASTCALL
#define FASTCALL	__fastcall		//!< 藉由暫存器 (eax, ecx) 加速 (x64 會做例外處理)
#endif

/**
 *	指標型態定義
 */
#ifndef NULL
#	if defined(ODMC_CPP)
#		define	NULL  0				//!< 空值，無值 null C++11 wnaana update to nullptr? (#define NULL nullptr)
#	else
#		define	NULL  ((void*)0)	//!< 空值，無值 null
#	endif
#endif

/**
 *	布林式運算值 TRUE / FALSE
 */
#ifndef TRUE
#define TRUE	1			//!< 布林式判別，值 = 1 (非零值)
#endif

#ifndef FALSE
#define FALSE	0			//!< 布林式判別，值 = 0
#endif

#endif // !ODMC_CONF_TYPEDEF_HH

