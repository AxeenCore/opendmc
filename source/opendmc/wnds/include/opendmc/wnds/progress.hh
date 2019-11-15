/**************************************************************************//**
 * @file	progress.hh
 * @brief	DmProgress 進度框類別 header
 * @date	2019-09-19
 * @date	2018-09-19
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_PROGRESS_HH
#define ODMC_WNDS_PROGRESS_HH
#include "wnds.hh"

/**
 *	@class DmProgressBar
 *	@brief 進度條類別
 */
class DmProgressBar : public DmWnds
{
public:
	DmProgressBar();
	virtual ~DmProgressBar();

	int			OffsetPos(int nInc) const;									// PBM_DELTAPOS
	COLORREF	GetBarColor() const;										// PBM_GETBARCOLOR
	COLORREF	GetBkColor() const;											// PBM_GETBKCOLOR
	int			GetPos() const;												// PBM_GETPOS
	int			GetRange(BOOL lowLimit) const;								// PBM_GETRANGE
	int			GetRange(BOOL lowLimit, PBRANGE* rangePtr) const;			// PBM_GETRANGE
	int			GetState() const;											// PBM_GETSTATE
	int			GetStep() const;											// PBM_GETSTEP
	COLORREF	SetBarColor(COLORREF colBar) const;							// PBM_SETBARCOLOR
	COLORREF	SetBkColor(COLORREF colBackground) const;					// PBM_SETBKCOLOR
	void		SetMarQuee(BOOL bSelectMode, DWORD dwMilliseconds) const;	// PBM_SETMARQUEE
	int			SetPos(int nPos) const;										// PBM_SETPOS
	DWORD		SetRange(DWORD dwRange) const;								// PBM_SETRANGE
	UINT32		SetRange32(UINT32 lowLimit, UINT32 hiLimit) const;			// PBM_SETRANGE32
	int			SetState(int nState) const;									// PBM_SETSTATE
	int			SetStep(int nInc) const;									// PBM_SETSTEP
	int			Stepit() const;												// PBM_STEPIT


private:
	DmProgressBar(const DmProgressBar&) = delete;				//!< Disable copy construction
	DmProgressBar& operator=(const DmProgressBar&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_PROGRESS_HH
