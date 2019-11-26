/**************************************************************************//**
 * @file	lpr_frame.hh
 * @brief	CxLprFrame 類別宣告 Header
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HSLPR_LPRFRAME_HH
#define	ODMC_HSLPR_LPRFRAME_HH
#include "lpr_define.hh"

/**
 *	@class CxLprFrame
 *	@brief CxLprFrame 主視窗類別
 */
class CxLprFrame : public DmWnds
{
public:
	CxLprFrame();
	virtual ~CxLprFrame();
	BOOL CreateSafe();

protected:
	virtual void SafeUserRelease() override;
	void WmUserCreate(WPARAM wParam, LPARAM lParam) override;
	LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;

private:
	CxLprFrame(const CxLprFrame&) = delete;				//!< Disable copy construction
	CxLprFrame& operator=(const CxLprFrame&) = delete;	//!< Disable assignment operator
	static void Rename(CxLprFrame* thisPtr);
	static void Detection(CxLprFrame* thisPtr);

	BOOL m_keepRename;
	BOOL m_keepDetection;
	cv::Mat m_cvMat;
	std::thread* m_threadRename;
	std::thread* m_threadDetection;
};

#endif // !ODMC_HSLPR_LPRFRAME_HH
