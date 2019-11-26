/**************************************************************************//**
 * @file	define.hh
 * @brief	HiSHARPLPR 定義檔
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HISHARPLPR_DEFINE_HH
#define	ODMC_HISHARPLPR_DEFINE_HH
#include "opendmc/opendmc_wnds.hh"
#include "opencv2/opencv.hpp"

#if defined(ODMC_WIN64)
#	if defined(ODMC_DEBUG)
#		pragma comment(lib, "opencv_world412d")
#	else
#		pragma comment(lib, "opencv_world412")
#	endif
#else
#	pragma message("Please using Win64 compiler...")
#endif

#define HSLPR_CLASSNAME	"HSLPR_CLASS"
#define	HSLPR_TITLENAME	"HS 車牌辨識 (台灣)"

#endif // !ODMC_HISHARPLPR_DEFINE_HH

