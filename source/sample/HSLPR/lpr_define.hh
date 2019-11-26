/**************************************************************************//**
 * @file	lpr_define.hh
 * @brief	HSLPR Header
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HSLPR_LPRDEFINE_HH
#define	ODMC_HSLPR_LPRDEFINE_HH
#include "opendmc/opendmc_wnds.hh"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio/videoio_c.h"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

#if defined(ODMC_DEBUG)
#	pragma comment(lib, "opencv_world412d")
#else
#	pragma comment(lib, "opencv_world412")
#endif

#define HSLPR_CLASSNAME	"HSLPR_CLASS"
#define	HSLPR_TITLENAME	"HS 車牌辨識 (台灣)"

#endif // !ODMC_HSLPR_LPRDEFINE_HH
