/**************************************************************************//**
 * @file	carplate.hh
 * @brief	CarPlate 前置定義 Header
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CARPLATE_CARPLATE_HH
#define	ODMC_CARPLATE_CARPLATE_HH
#include "opendmc/opendmc_wnds.hh"
#include "opendmc/opendmc_image.hh"
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

//#define CAR_PLATE_USING_VIDEOFILE

#define CAR_PLATE_CLASSEX	"HISHARP_CARPLATE"
#define CAR_PLATE_CAPTION	"HS 車牌辨識"
#define CAR_PLATE_MAINFORM	"49AAD935-8E45-4F9C-98CB-88FBC26E1EB1"

#define CAR_PLATE_SOLUTION		3	//!< 指定何種形式處理車牌偵測
#define CAR_PLATE_ERROR_LIMIT	5	//!< 連續發生錯誤上限值

#define CAR_PLATE_OLDPLATE_WD	32	//!< 舊式車牌寬度 (cm)
#define	CAR_PLATE_OLDPLATE_HT	15	//!< 舊式車牌高度 (cm)

#define CAR_PLATE_NEWPLATE_WD	38	//!< 新式車牌寬度 (cm)
#define	CAR_PLATE_NEWPLATE_HT	16	//!< 新式車牌高度 (cm)

#define CAR_PLATE_BLUE_WD		11	//!< 進行平滑寬度
#define	CAR_PLATE_BLUE_HT		5	//!< 進行平滑高度

#define CAR_PLATE_THRESH_BLOCK_SIZE	19
#define CAR_PLATE_THRESH_WEIGHT		9

#endif // !ODMC_CARPLATE_CARPLATE_HH
