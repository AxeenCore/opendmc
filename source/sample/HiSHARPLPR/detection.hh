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

extern cv::VideoCapture* OpenVideoCapture(const cv::String& url);
extern void TransferToWindow(HWND hWnd, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr, bool haff=false);
extern void FrameToContrast(cv::Mat& frame);
extern void FrameToSobel(cv::Mat& frame);
extern void FrameToCanny(cv::Mat& frame);
extern void FrameErosionDilation(cv::Mat frame);
extern void VideoCaptureProcess(cv::Mat& buffer, std::mutex& mutex, void* keepPtr, void* restPtr);
#endif // !ODMC_HISHARPLPR_DETECTION_HH
