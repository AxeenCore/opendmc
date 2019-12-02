/**************************************************************************//**
 * @file	frame.hh
 * @brief	LPRFrame 類別宣告
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HISHARPLPR_FRAME_HH
#define	ODMC_HISHARPLPR_FRAME_HH
#include "detection.hh"

#define USES_VIDEO_MODE

/**
 *	@class LPRFrame
 *	@brief LPRFrame 主視窗類別
 */
class LPRFrame : public DmWnds
{
public:
	LPRFrame();
	virtual ~LPRFrame();
	BOOL CreateSafeWindow();

protected:
	virtual void SafeUserRelease() override;
	void WmClose(WPARAM wParam, LPARAM lParam) override;
	void WmUserCreate(WPARAM wParam, LPARAM lParam) override;
	LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;

	void DetectionFormPicture(cv::String& filename);

	void EndDetection();
	static void DetectionProcess(LPRFrame* framePtr);

private:
	LPRFrame(const LPRFrame&) = delete;				//!< Disable copy construction
	LPRFrame& operator=(const LPRFrame&) = delete;	//!< Disable assignment operator

	// 成員宣告
	BOOL m_keeptoDetection;
	std::thread* m_threadDetection;
};

#endif // !ODMC_HISHARPLPR_FRAME_HH
