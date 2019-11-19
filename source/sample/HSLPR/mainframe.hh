/**************************************************************************//**
 * @file	mainframe.hh
 * @brief	CxMainframe 類別宣告
 * @date	2019-11-18
 * @date	2019-11-18
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HSLPR_MAINFRAME_HH
#define	ODMC_HSLPR_MAINFRAME_HH
#include "define.hh"

/**
 *	@class CxMainframe
 *	@brief CxMainframe HSLPR 主視窗
 */
class CxMainframe : public DmWnds
{
public:
	CxMainframe();
	virtual ~CxMainframe();

protected:
	// 基底虛擬函數
	//	virtual void	DoSafeRelease() = 0;
	//	virtual LRESULT	WmNcCreate(WPARAM wParam, LPARAM lParam);
	//	virtual LRESULT	WmCreate(WPARAM wParam, LPARAM lParam);
	//	virtual void	WmDestroy(WPARAM wParam, LPARAM lParam);
	//	virtual void	WmNcDestroy(WPARAM wParam, LPARAM lParam);
	//	virtual	void	WmClose(WPARAM wParam, LPARAM lParam);
	//	virtual void	WmUserCreate(WPARAM wParam, LPARAM lParam);
	//	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	virtual void	SafeUserRelease() override;
	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;
};




#endif // !ODMC_HSLPR_MAINFRAME_HH
