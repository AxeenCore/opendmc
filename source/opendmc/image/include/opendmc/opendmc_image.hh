/**************************************************************************//**
 * @file	opendmc_wnds.hh
 * @brief	opendmc_wnds.lib header
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_OPENDMC_IMAGE_HH
#define	ODMC_OPENDMC_IMAGE_HH
#include "image/surface.hh"

#if defined(ODMC_WINDOWS) && defined(ODMC_WIN64)
#	if defined(ODMC_DEBUG)
#		pragma comment(lib, "x64/debug/opendmc_image")
#	else
#		pragma comment(lib, "x64/release/opendmc_image")
#	endif
#else
#	if defined(ODMC_DEBUG)
#		pragma comment(lib, "x86/debug/opendmc_image");
#	else
#		pragma comment(lib, "x86/release/opendmc_image");
#	endif
#endif

#endif // !ODMC_OPENDMC_IMAGE_HH
