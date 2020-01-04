/**************************************************************************//**
 * @file	dialog.hh
 * @brief	DmDialog 對話框類別宣告 Header
 * @date	2000-10-10
 * @date	2018-08-10
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_DIALOG_HH
#define	ODMC_WNDS_DIALOG_HH
#include "wnds.hh"

/**
 *	@class DmDialog
 *	@brief 對話框類別宣告
 */
class DmDialog : public DmWnds
{
public:
	DmDialog();
	virtual ~DmDialog();

	virtual INT_PTR DoModal(const TCHAR* szTemplatePtr, HWND hWndParent);
	virtual INT_PTR DoModal(int nIDDItem, HWND hWndParent);
	virtual BOOL DoModeless(const TCHAR* szTemplatePtr, HWND hWndParent);
	virtual BOOL DoModeless(int nIDDItem, HWND hWndParent);
	virtual LONG_PTR GetMineCallback() const override;

protected:
	INT_PTR DefaultDlgProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
	virtual BOOL WmInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void WmUserCreate(WPARAM wParam, LPARAM lParam) override;
	virtual INT_PTR DlgProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	BOOL BindWindow(HWND hWnd) final;
	void LooseWindow() final;

private:
	DmDialog(const DmDialog&) = delete;				//!< Disable copy construction
	DmDialog& operator=(const DmDialog&) = delete;	//!< Disable assignment operator
	static INT_PTR CALLBACK StaticDlgProc(HWND hWndDlg, UINT uMessage, WPARAM wParam, LPARAM lParam);
};

#endif // !ODMC_WNDS_DIALOG_HH
