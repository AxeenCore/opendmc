/**************************************************************************//**
 * @file	undef.hh
 * @brief	Undefine windows api macros
 * @date	2010-03-25
 * @date	2018-08-31
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CONF_UNFUNCDEF_HH
#define ODMC_CONF_UNFUNCDEF_HH

#if defined(ODMC_WINDOWS)

#ifdef CallWindowProc
#undef CallWindowProc
#endif
inline LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::CallWindowProcW(lpPrevWndFunc, hWnd, uMsg, wParam, lParam);
#else
	return ::CallWindowProcA(lpPrevWndFunc, hWnd, uMsg, wParam, lParam);
#endif
}


#ifdef CreateDialog
#undef CreateDialog
#endif
inline HWND CreateDialog(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
#ifdef ODMC_UNICODE
	return ::CreateDialogParamW(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#else
	return ::CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#endif
}


#ifdef CreateFont
#undef CreateFont
#endif
inline HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCTSTR lpszFace)
{
#ifdef ODMC_UNICODE
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
#else
	return CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
#endif
}


#ifdef CreateFontIndirect
#undef CreateFontIndirect
#endif
inline HFONT CreateFontIndirect(const LOGFONT *lplf)
{
#ifdef ODMC_UNICODE
	return ::CreateFontIndirectW(lplf);
#else
	return ::CreateFontIndirectA(lplf);
#endif
}


#ifdef CreateWindow
#undef CreateWindow
#endif
inline HWND WINAPI CreateWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
#ifdef ODMC_UNICODE
	return ::CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#else
	return ::CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#endif
}


#ifdef CreateWindowEx
#undef CreateWindowEx
#endif
inline HWND WINAPI CreateWindowEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
#ifdef ODMC_UNICODE
	return ::CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#else
	return ::CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#endif
}


#ifdef DefWindowProc
#undef DefWindowProc
#endif
inline LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
#else
	return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
#endif
}


#ifdef DialogBoxParam
#undef DialogBoxParam
#endif
inline INT_PTR DialogBoxParam(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
#ifdef ODMC_UNICODE
	return DialogBoxParamW(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#else
	return DialogBoxParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#endif
}


#ifdef CreateDialogParam
#undef CreateDialogParam
#endif
inline HWND CreateDialogParam(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
#ifdef ODMC_UNICODE
	return CreateDialogParamW(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#else
	return CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
#endif
}


#ifdef DlgDirList
#undef DlgDirList
#endif
inline int DlgDirList(HWND hDlg, LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType)
{
#ifdef ODMC_UNICODE
	return ::DlgDirListW(hDlg, lpPathSpec, nIDListBox, nIDStaticPath, uFileType);
#else
	return ::DlgDirListA(hDlg, lpPathSpec, nIDListBox, nIDStaticPath, uFileType);
#endif
}


#ifdef DlgDirListComboBox
#undef DlgDirListComboBox
#endif
inline int DlgDirListComboBox(HWND hDlg, LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype)
{
#ifdef ODMC_UNICODE
	return ::DlgDirListComboBoxW(hDlg, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
#else
	return ::DlgDirListComboBoxA(hDlg, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
#endif
}


#ifdef DlgDirSelectEx
#undef DlgDirSelectEx
#endif
inline BOOL DlgDirSelectEx(HWND hwndDlg, LPTSTR lpString, int chCount, int idListBox)
{
#ifdef ODMC_UNICODE
	return ::DlgDirSelectExW(hwndDlg, lpString, chCount, idListBox);
#else
	return ::DlgDirSelectExA(hwndDlg, lpString, chCount, idListBox);
#endif
}


#ifdef DlgDirSelectComboBoxEx
#undef DlgDirSelectComboBoxEx
#endif
inline BOOL DlgDirSelectComboBoxEx(HWND hwndDlg, LPTSTR lpString, int cchOut, int idComboBox)
{
#ifdef ODMC_UNICODE
	return ::DlgDirSelectComboBoxExW(hwndDlg, lpString, cchOut, idComboBox);
#else
	return ::DlgDirSelectComboBoxExA(hwndDlg, lpString, cchOut, idComboBox);
#endif
}


#ifdef FindWindow
#undef FindWindow
#endif
inline HWND FindWindow(const TCHAR* lpClassName, const TCHAR* lpWindowName)
{
#ifdef ODMC_UNICODE
	return ::FindWindowW(lpClassName, lpWindowName);
#else
	return ::FindWindowA(lpClassName, lpWindowName);
#endif
}


#ifdef FindWindowEx
#undef FindWindowEx
#endif
inline HWND FindWindowEx(HWND hWndParent, HWND hWndChildAfter, const TCHAR* szClassNamePtr, const TCHAR* szWindowNamePtr)
{
#ifdef ODMC_UNICODE
	return ::FindWindowExW(hWndParent, hWndChildAfter, szClassNamePtr, szWindowNamePtr);
#else
	return ::FindWindowExA(hWndParent, hWndChildAfter, szClassNamePtr, szWindowNamePtr);
#endif
}


#ifdef FormatMessage
#undef FormatMessage
#endif
inline DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPTSTR lpBuffer, DWORD nSize, va_list *Arguments)
{
#ifdef ODMC_UNICODE
	return ::FormatMessageW(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
#else
	return ::FormatMessageA(dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, Arguments);
#endif
}


#ifdef GetClassLong
#undef GetClassLong
#endif
inline DWORD WINAPI GetClassLong(HWND hWnd, int nIndex)
{
#ifdef ODMC_UNICODE
	return ::GetClassLongW(hWnd, nIndex);
#else
	return ::GetClassLongA(hWnd, nIndex);
#endif
}


#ifdef GetClassLongPtr
#undef GetClassLongPtr
#endif
inline ULONG_PTR WINAPI GetClassLongPtr(HWND hWnd, int nIndex)
{
#ifdef ODMC_UNICODE
	return ::GetClassLongPtrW(hWnd, nIndex);
#else
	return ::GetClassLongPtrA(hWnd, nIndex);
#endif
}


#ifdef GetClassInfo
#undef GetClassInfo
#endif
inline BOOL GetClassInfo(HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASSW lpWndClass)
{
#ifdef ODMC_UNICODE
	return ::GetClassInfoW(hInstance, lpClassName, lpWndClass);
#else
	return ::GetClassInfoA(hInstance, lpClassName, lpWndClass);
#endif
}


#ifdef GetClassInfoEx
#undef GetClassInfoEx
#endif
inline BOOL GetClassInfoEx(HINSTANCE hInstance, LPCTSTR lpszClass, LPWNDCLASSEXW lpwcx)
{
#ifdef ODMC_UNICODE
	return ::GetClassInfoExW(hInstance, lpszClass, lpwcx);
#else
	return ::GetClassInfoExA(hInstance, lpszClass, lpwcx);
#endif
}


#ifdef GetClassName
#undef GetClassName
#endif
inline int GetClassName(HWND hWnd, LPTSTR lpClassName, int nMaxCount)
{
#ifdef ODMC_UNICODE
	return ::GetClassNameW(hWnd, lpClassName, nMaxCount);
#else
	return ::GetClassNameA(hWnd, lpClassName, nMaxCount);
#endif
}


#ifdef GetDlgItemText
#undef GetDlgItemText
#endif
inline UINT GetDlgItemText(HWND hDlg, int nIDDlgItem, LPTSTR lpString, int cchMax)
{
#ifdef ODMC_UNICODE
	return ::GetDlgItemTextW(hDlg, nIDDlgItem, lpString, cchMax);
#else
	return ::GetDlgItemTextA(hDlg, nIDDlgItem, lpString, cchMax);
#endif
}


#ifdef GetEnhMetaFile
#undef GetEnhMetaFile
#endif
inline HENHMETAFILE GetEnhMetaFile(LPCTSTR lpName)
{
#ifdef ODMC_UNICODE
	return ::GetEnhMetaFileW(lpName);
#else
	return ::GetEnhMetaFileA(lpName);
#endif
}


#ifdef GetModuleFileName
#undef GetModuleFileName
#endif
inline DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize)
{
#ifdef ODMC_UNICODE
	return ::GetModuleFileNameW(hModule, lpFilename, nSize);
#else
	return ::GetModuleFileNameA(hModule, lpFilename, nSize);
#endif
}


#ifdef GetModuleHandle
#undef GetModuleHandle
#endif
inline HMODULE WINAPI GetModuleHandle(LPCTSTR lpModuleName)
{
#ifdef ODMC_UNICODE
	return ::GetModuleHandleW(lpModuleName);
#else
	return ::GetModuleHandleA(lpModuleName);
#endif
}


#ifdef GetWindowLong
#undef GetWindowLong
#endif
inline LONG GetWindowLong(HWND hWnd, int  nIndex)
{
#ifdef ODMC_UNICODE
	return ::GetWindowLongW(hWnd, nIndex);
#else
	return ::GetWindowLongA(hWnd, nIndex);
#endif
}


#ifdef GetWindowLongPtr
#undef GetWindowLongPtr
#endif
inline LONG_PTR GetWindowLongPtr(HWND hWnd, int nIndex)
{
#ifdef ODMC_UNICODE
	return ::GetWindowLongPtrW(hWnd, nIndex);
#else
	return ::GetWindowLongPtrA(hWnd, nIndex);
#endif
}


#ifdef GetWindowText
#undef GetWindowText
#endif
inline int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount)
{
#ifdef ODMC_UNICODE
	return ::GetWindowTextW(hWnd, lpString, nMaxCount);
#else
	return ::GetWindowTextA(hWnd, lpString, nMaxCount);
#endif
}


#ifdef GetWindowTextLength
#undef GetWindowTextLength
#endif
inline int GetWindowTextLength(HWND hWnd)
{
#ifdef ODMC_UNICODE
	return ::GetWindowTextLengthW(hWnd);
#else
	return ::GetWindowTextLengthA(hWnd);
#endif
}


#ifdef IsDialogMessage
#undef IsDialogMessage
#endif
inline BOOL IsDialogMessage(HWND hDlg, LPMSG lpMsg)
{
#ifdef ODMC_UNICODE
	return ::IsDialogMessageW(hDlg, lpMsg);
#else
	return ::IsDialogMessageA(hDlg, lpMsg);
#endif
}


#ifdef LoadIcon
#undef LoadIcon
#endif
inline HICON LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName)
{
#ifdef ODMC_UNICODE
	return ::LoadIconW(hInstance, lpIconName);
#else
	return ::LoadIconA(hInstance, lpIconName);
#endif
}


#ifdef MessageBox
#undef MessageBox
#endif
inline int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
#ifdef ODMC_UNICODE
	return ::MessageBoxW(hWnd, lpText, lpCaption, uType);
#else
	return ::MessageBoxA(hWnd, lpText, lpCaption, uType);
#endif
}


#ifdef OutputDebugString
#undef OutputDebugString
#endif
inline void WINAPI OutputDebugString(LPCTSTR lpOutputString)
{
#ifdef ODMC_UNICODE
	::OutputDebugStringW(lpOutputString);
#else
	::OutputDebugStringA(lpOutputString);
#endif
}


#ifdef PeekMessage
#undef PeekMessage
#endif
inline BOOL PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
#ifdef ODMC_UNICODE
	return ::PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
#else
	return ::PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
#endif
}


#ifdef PostMessage
#undef PostMessage
#endif
inline BOOL WINAPI PostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::PostMessageW(hWnd, Msg, wParam, lParam);
#else
	return ::PostMessageA(hWnd, Msg, wParam, lParam);
#endif
}


#ifdef RegisterClass
#undef RegisterClass
#endif
inline ATOM WINAPI RegisterClass(const WNDCLASS* pwc)
{
#ifdef ODMC_UNICODE
	return ::RegisterClassW(pwc);
#else
	return ::RegisterClassA(pwc);
#endif
}


#ifdef RegisterClassEx
#undef RegisterClassEx
#endif
inline ATOM WINAPI RegisterClassEx(const WNDCLASSEX *pwcex)
{
#ifdef ODMC_UNICODE
	return ::RegisterClassExW(pwcex);
#else
	return ::RegisterClassExA(pwcex);
#endif
}


#ifdef RegisterWindowMessage
#undef RegisterWindowMessage
#endif
inline UINT RegisterWindowMessage(LPCTSTR lpString)
{
#ifdef ODMC_UNICODE
	return ::RegisterWindowMessageW(lpString);
#else
	return ::RegisterWindowMessageA(lpString);
#endif
}


#ifdef SendDlgItemMessage
#undef SendDlgItemMessage
#endif
inline LRESULT SendDlgItemMessage(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::SendDlgItemMessageW(hDlg, nIDDlgItem, Msg, wParam, lParam);
#else
	return ::SendDlgItemMessageA(hDlg, nIDDlgItem, Msg, wParam, lParam);
#endif
}


#ifdef SendMessage
#undef SendMessage
#endif
inline LRESULT WINAPI SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::SendMessageW(hWnd, uMsg, wParam, lParam);
#else
	return ::SendMessageA(hWnd, uMsg, wParam, lParam);
#endif
}


#ifdef SendNotifyMessage
#undef SendNotifyMessage
#endif
inline BOOL SendNotifyMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
#ifdef ODMC_UNICODE
	return ::SendNotifyMessageW(hWnd, Msg, wParam, lParam);
#else
	return ::SendNotifyMessageA(hWnd, Msg, wParam, lParam);
#endif
}


#ifdef SetClassLong
#undef SetClassLong
#endif
inline DWORD SetClassLong(HWND hWnd, int nIndex, LONG dwNewLong)
{
#ifdef ODMC_UNICODE
	return ::SetClassLongW(hWnd, nIndex, dwNewLong);
#else
	return ::SetClassLongA(hWnd, nIndex, dwNewLong);
#endif
}


#ifdef SetClassLongPtr
#undef SetClassLongPtr
#endif
inline ULONG_PTR SetClassLongPtr(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
#ifdef ODMC_UNICODE
	return ::SetClassLongPtrW(hWnd, nIndex, dwNewLong);
#else
	return ::SetClassLongPtrA(hWnd, nIndex, dwNewLong);
#endif
}


#ifdef SetDlgItemText
#undef SetDlgItemText
#endif
inline BOOL SetDlgItemText(HWND hDlg, int nIDDlgItem, LPCTSTR lpString)
{
#ifdef ODMC_UNICODE
	return ::SetDlgItemTextW(hDlg, nIDDlgItem, lpString);
#else
	return ::SetDlgItemTextA(hDlg, nIDDlgItem, lpString);
#endif
}


#ifdef SetWindowLong
#undef SetWindowLong
#endif
inline LONG SetWindowLong(HWND hWnd, int nIndex, LONG dwNewLong)
{
#ifdef ODMC_UNICODE
	return ::SetWindowLongW(hWnd, nIndex, dwNewLong);
#else
	return ::SetWindowLongA(hWnd, nIndex, dwNewLong);
#endif
}


#ifdef SetWindowLongPtr
#undef SetWindowLongPtr
#endif
inline LONG_PTR SetWindowLongPtr(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
#ifdef ODMC_UNICODE
	return ::SetWindowLongPtrW(hWnd, nIndex, dwNewLong);
#else
	return ::SetWindowLongPtrA(hWnd, nIndex, dwNewLong);
#endif
}


#ifdef SetWindowText
#undef SetWindowText
#endif
inline BOOL SetWindowText(HWND hWnd, LPCTSTR lpString)
{
#ifdef ODMC_UNICODE
	return ::SetWindowTextW(hWnd, lpString);
#else
	return ::SetWindowTextA(hWnd, lpString);
#endif
}


#ifdef UnregisterClass
#undef UnregisterClass
#endif
inline BOOL UnregisterClass(LPCTSTR lpClassName, HINSTANCE hInstance)
{
#ifdef ODMC_UNICODE
	return ::UnregisterClassW(lpClassName, hInstance);
#else
	return ::UnregisterClassA(lpClassName, hInstance);
#endif
}

#endif	// !ODMC_WINDOWS
#endif	// !ODMC_CONF_UNFUNCDEF_HH
