/**************************************************************************//**
 * @file	detection.hh
 * @brief	車牌偵測函數宣告 Header
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HISHARPLPR_DETECTION_HH
#define ODMC_HISHARPLPR_DETECTION_HH
#include "define.hh"

extern void SetBMPInfo(BITMAPINFO* biPtr, int wd, int ht);
extern void ShowSurface(HWND hWnd, int x, int y, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr);

extern cv::VideoCapture* OpenVideoCapture(const cv::String& url);
extern void FrameToContrast(cv::Mat& frame);
extern void FrameToSobel(cv::Mat& frame);
extern void FrameToCanny(cv::Mat& frame);
extern void FrameCloseOperation(cv::Mat& frame);
extern void getMaxArea(cv::Mat& frame, cv::Mat& closeImage);
extern void VideoCaptureProcess(cv::Mat& buffer, std::mutex& mutex, void* keepPtr, void* restPtr);
#endif // !ODMC_HISHARPLPR_DETECTION_HH
