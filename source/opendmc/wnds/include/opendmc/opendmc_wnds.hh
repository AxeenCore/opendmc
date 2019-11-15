/**************************************************************************//**
 * @file	opendmc_wnds.hh
 * @brief	opendmc_wnds.lib header
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_OPENDMC_WNDS_HH
#define	ODMC_OPENDMC_WNDS_HH

#include "wnds/frame.hh"
#include "wnds/dialog.hh"
#include "wnds/button.hh"
#include "wnds/combo.hh"
#include "wnds/editbox.hh"
#include "wnds/imagelist.hh"
#include "wnds/listbox.hh"
#include "wnds/listview.hh"
#include "wnds/progress.hh"
#include "wnds/richedit.hh"
#include "wnds/tabview.hh"
#include "wnds/treeview.hh"

#if defined(ODMC_WINDOWS)
#	// Windows operating system library
#	pragma comment(lib, "kernel32")
#	pragma comment(lib, "user32")
#	pragma comment(lib, "gdi32")
#	pragma comment(lib, "winspool")
#	pragma comment(lib, "comdlg32")
#	pragma comment(lib, "advapi32")
#	pragma comment(lib, "shell32")
#	pragma comment(lib, "ole32")
#	pragma comment(lib, "oleaut32")
#	pragma comment(lib, "uuid")
#	pragma comment(lib, "odbc32")
#	pragma comment(lib, "odbccp32")
#
#	pragma comment(lib, "comctl32")
#	pragma comment(lib, "winmm")
#	pragma comment(lib, "shlwapi")
#
#	if defined(ODMC_WIN64)
#		if defined(ODMC_DEBUG)
#			pragma comment(lib, "x64/debug/opendmc_wnds")
#		else
#			pragma comment(lib, "x64/release/opendmc_wnds")
#		endif
#	else
#		if defined(ODMC_DEBUG)
#			pragma comment(lib, "x86/debug/opendmc_wnds");
#		else
#			pragma comment(lib, "x86/release/opendmc_wnds");
#		endif
#	endif
#endif

#endif // !ODMC_OPENDMC_WNDS_HH
