/**************************************************************************//**
 * @file	main.cc
 * @brief	Sample Window main function
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#include "apps.hh"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPTSTR szCmdPtr, int iCmdShow) {
	auto dmApps = new (std::nothrow) CxApps();
	auto exitCode = 0;

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hInstPrev);
	UNREFERENCED_PARAMETER(szCmdPtr);
	UNREFERENCED_PARAMETER(iCmdShow);

	if (dmApps != NULL) {
		exitCode = dmApps->Run();
		SAFE_DELETE(dmApps);
	}
	return exitCode;
}
