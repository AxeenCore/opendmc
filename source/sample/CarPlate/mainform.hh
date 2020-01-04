/**************************************************************************//**
 * @file	mainform.hh
 * @brief	CarMainform 類別宣告 Header
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CARPLATE_MAINFORM_HH
#define	ODMC_CARPLATE_MAINFORM_HH
#include "carplate.hh"
#include "detection.hh"

/**
 *	@class CarMainform
 *	@brief 車牌辨識主視窗 (主框架)
 */
class CarMainform : public DmWnds
{
public:
	CarMainform();
	virtual ~CarMainform();
	BOOL CreateMine() override;

protected:
	void SafeUserRelease() override;

	// 訊息處理
	void WmClose(WPARAM wParam, LPARAM lParam) override;
	void WmUserCreate(WPARAM wParam, LPARAM lParam) override;
	LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;

private:
	CarMainform(const CarMainform&) = delete;				//!< Disable copy construction
	CarMainform& operator=(const CarMainform&) = delete;	//!< Disable assignment operator
	
	// 變數、物件成員宣告 
	CarPlateDetection* m_cDetection;						//!< 車牌偵測 object
};

#endif // !ODMC_CARPLATE_MAINFORM_HH
