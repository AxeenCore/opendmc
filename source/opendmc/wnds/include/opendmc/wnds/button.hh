/**************************************************************************//**
 * @file	button.hh
 * @brief	DmButton 按鈕類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_BUTTON_HH
#define ODMC_WNDS_BUTTON_HH
#include "wnds.hh"

/**
 *	@class DmButton
 *	@brief 標準按鈕類別
 */
class DmButton : public DmWnds
{
public:
	DmButton();
	virtual ~DmButton();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample() override;

	// Operations
	BOOL	GetIdealSize(SIZE* sizePtr) const;								// BCM_GETIDEALSIZE
	BOOL	GetImageList(BUTTON_IMAGELIST* imePtr) const;					// BCM_GETIMAGELIST
	BOOL	GetNote(LPTSTR szNotePtr, DWORD* ccNotePtr) const;				// BCM_GETNOTE
	DWORD	GetNoteLength() const;											// BCM_GETNOTELENGTH
	BOOL	GetSplitInfo(BUTTON_SPLITINFO* splitPtr) const;					// BCM_GETSPLITINFO
	BOOL	GetTextMargin(RECT* rcPtr) const;								// BCM_GETTEXTMARGIN
	BOOL	SetDropDownState(BOOL bState) const;							// BCM_SETDROPDOWNSTATE
	BOOL	SetImageList(const PBUTTON_IMAGELIST* btnImageListPtr) const;	// BCM_SETIMAGELIST
	BOOL	SetNote(const TCHAR* szNotePtr) const;							// BCM_SETNOTE
	BOOL	SetElevationRequired(BOOL bRequired) const;						// BCM_SETSHIELD
	BOOL	SetSplitInfo(BUTTON_SPLITINFO* splitPtr) const;					// BCM_SETSPLITINFO
	BOOL	SetTextMargin(RECT* rcPtr) const;								// BCM_SETTEXTMARGIN

	void	Click() const;													// BM_CLICK
	LRESULT	GetCheck() const;												// BM_GETCHECK
	LRESULT	GetImage(int nType) const;										// BM_GETIMAGE
	HBITMAP	GetButtonBitmap() const;										// ## BM_GETIMAGE
	HCURSOR	GetButtonCursor() const;										// ## BM_GETIMAGE
	HICON	GetButtonIcon() const;											// ## BM_GETIMAGE
	LRESULT GetState() const;												// BM_GETSTATE
	UINT	GetButtonStyle() const;											// ## ---
	LRESULT SetCheck(int nCheck) const;										// BM_SETCHECK
	void	SetRadioClick(BOOL bState) const;								// BM_SETDONTCLICK
	LRESULT	SetImage(int nType, void* vPtr) const;							// BM_SETIMAGE
	HBITMAP	SetButtonBitmap(HBITMAP hBitmap) const;							// ## BM_SETIMAGE
	HCURSOR	SetButtonCursor(HCURSOR hCursor) const;							// ## BM_SETIMAGE
	HICON	SetButtonIcon(HICON hIcon) const;								// ## BM_SETIMAGE
	LRESULT SetState(BOOL bState) const;									// BM_SETSTATE
	void	SetButtonStyle(DWORD dwStyle, BOOL bRepaint) const;				// BM_SETSTYLE

private:
	DmButton(const DmButton&) = delete;				//!< Disable copy construction
	DmButton& operator=(const DmButton&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_BUTTON_HH
