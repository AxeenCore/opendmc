/**************************************************************************//**
 * @file	lpr_main.hh
 * @brief	Sample Window 程序入口
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "lpr_apps.hh"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPTSTR szCmdPtr, int iCmdShow) {
	auto cApps = new (std::nothrow) CxLprApps();
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
