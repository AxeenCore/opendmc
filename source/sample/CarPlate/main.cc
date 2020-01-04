/**************************************************************************//**
 * @file	main.cc
 * @brief	CarPlate 程式進入口
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#include "apps.hh"

/**
 *	@brief	Windows 程式入口函數
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPTSTR szCmdPtr, int iCmdShow) {
	auto cApps = new (std::nothrow) AppsCarPlate();
	auto exitCode = 0;

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hInstPrev);
	UNREFERENCED_PARAMETER(szCmdPtr);
	UNREFERENCED_PARAMETER(iCmdShow);

	if (cApps != NULL) {
		exitCode = cApps->Run();
		SAFE_DELETE(cApps);
	}
	return exitCode;
}

