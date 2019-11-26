/**************************************************************************//**
 * @file	imagelist.hh
 * @brief	DmImageList 圖式列表類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-15
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_IMAGELIST_HH
#define	ODMC_WNDS_IMAGELIST_HH
#include "object.hh"

/**
 *	@class DmImageList
 *	@brief Windows 圖示列表類別
 */
class DmImageList : public DmWndsObject
{
public:
	DmImageList();
	virtual ~DmImageList();

	int Add(HBITMAP hbmImage, HBITMAP hbmMask) const;
	int Add(HBITMAP hbmImage, COLORREF colMask) const;
	int Add(HICON hIcon) const;
	void Attach(HIMAGELIST hIml);
	BOOL BeginDrag(int nImage, POINT* HotSpotPtr) const;
	BOOL Copy(int iDst, int iSrc, UINT uFlags = ILCF_MOVE) const;
	BOOL DeleteImageList();
	HIMAGELIST Detach();

	void Release();
	BOOL IsImageList() const { return m_hIml != NULL; }
	operator HIMAGELIST () const { return m_hIml; }


	// MFC CImageList member
	// CImageList::Add					將映像或映像加入至影像清單中。
	// CImageList::Attach				附加至影像清單CImageList物件。
	// CImageList::BeginDrag			開始拖曳影像。
	// CImageList::Copy					將映像內複製CImageList物件。
	// CImageList::Create				初始化的映像清單，並將它附加至CImageList物件。
	// CImageList::DeleteImageList		刪除映像清單。
	// CImageList::DeleteTempMap		由呼叫CWinApp閒置時間處理常式，以刪除任何暫存CImageList所建立的物件FromHandle。
	// CImageList::Detach				從影像清單物件中斷連結CImageList物件，並傳回影像清單的控制代碼。
	// CImageList::DragEnter			在拖曳作業期間鎖定更新，並顯示拖曳影像的指定位置。
	// CImageList::DragLeave			解除鎖定 視窗，並隱藏拖曳影像，以便可以更新 視窗。
	// CImageList::DragMove				將拖放作業期間被拖曳的影像。
	// CImageList::DragShowNolock		顯示或隱藏在拖曳作業時，拖曳影像，而不需要鎖定的視窗。
	// CImageList::Draw					繪製拖放作業期間被拖曳的影像。
	// CImageList::DrawEx				在指定的裝置內容中繪製的映像清單項目。 函式會使用指定的繪製樣式，並混合的映像，以指定的色彩。
	// CImageList::DrawIndirect			從影像清單中繪製影像。
	// CImageList::EndDrag				結束拖曳作業。
	// CImageList::ExtractIcon			會建立為基礎的影像和遮罩影像清單中的圖示。
	// CImageList::FromHandle			將指標傳回至CImageList物件時指定映像清單的控制代碼。 如果 CImageList 物件沒有附加至控制代碼，會建立並附加暫存 CImageList 物件。
	// CImageList::FromHandlePermanent	將指標傳回至CImageList物件時指定映像清單的控制代碼。 如果CImageList物件沒有附加至控制代碼，則會傳回 NULL。
	// CImageList::GetBkColor			擷取目前的背景色彩，影像清單。
	// CImageList::GetDragImage			取得用於拖曳的暫存影像清單。
	// CImageList::GetImageCount		擷取映像清單中的映像數目。
	// CImageList::GetImageInfo			擷取映像的相關資訊。
	// CImageList::GetSafeHandle		擷取m_hImageList。
	// CImageList::Read					從封存讀取影像清單。
	// CImageList::Remove				從影像清單中移除映像。
	// CImageList::Replace				利用新影像取代影像清單中的映像。
	// CImageList::SetBkColor			設定影像清單的背景色彩。
	// CImageList::SetDragCursorImage	建立新的拖曳影像。
	// CImageList::SetImageCount		重設影像清單中的映像的計數。
	// CImageList::SetOverlayImage		將映像當做覆疊遮罩清單中的影像之以零為起始的索引。
	// CImageList::Write				寫入封存中的影像清單。

protected:
	void SetSelfHandle(HIMAGELIST hIml) { m_hIml = hIml; }
	HIMAGELIST	m_hIml;		//!< 圖示列表操作代碼

protected:
	DmImageList(const DmImageList&) = delete;				//!< Disable copy construction
	DmImageList& operator=(const DmImageList&) = delete;	//!< Disable assignment operator
};


#endif // !ODMC_WNDS_IMAGELIST_HH
