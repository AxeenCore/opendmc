/**************************************************************************//**
 * @file	imagelist.cc
 * @brief	DmImageList 圖式列表類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-15
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/imagelist.hh"

/**
 * DmImageList 建構式
 */
DmImageList::DmImageList() : DmWndsObject(), m_hIml(NULL) { }

/**
 * DmImageList 解構式
 */
DmImageList::~DmImageList() { }

/**
 *	@brief 新增圖示至列表
 *	@param[in] hbmImage	要新增的圖像操作代碼
 *	@param[in] hbmMask	圖項遮罩操作代碼，此時允許為 NULL
 *	@return <b>型別: int</b> \n 若運作成功返回值為新增的圖像索引值，若運作失敗則返回值為 (-1)
 */
int DmImageList::Add(HBITMAP hbmImage, HBITMAP hbmMask) const
{
	if (this->IsImageList()) {
		return ::ImageList_Add(m_hIml, hbmImage, hbmMask);
	}
	return -1;
}

/**
 *	@brief 新增圖示至列表
 *	@param[in] hbmImage	要新增的圖像操作代碼
 *	@param[in] colMask	預遮罩的顏色值
 *	@return <b>型別: int</b> \n 若運作成功返回值為新增的圖像索引值，若運作失敗則返回值為 (-1)
 */
int DmImageList::Add(HBITMAP hbmImage, COLORREF colMask) const
{
	if (this->IsImageList()) {
		return ::ImageList_AddMasked(m_hIml, hbmImage, colMask);
	}
	return -1;
}

/**
 *	@brief 新增圖示至列表
 *	@param[in] hIcon 要新增的圖示操作代碼
 *	@return <b>型別: int</b> \n 若運作成功返回值為新增的圖像索引值，若運作失敗則返回值為 (-1)
 */
int DmImageList::Add(HICON hIcon) const
{
	if (this->IsImageList()) {
		return ::ImageList_ReplaceIcon(m_hIml, -1, hIcon);
	}
	return -1;
}

/**
 *	@brief 連接已存在的 Image List
 *	@param[in] hIml 要連接的 Image list 操作代碼
 *	@return 此函數沒有返回值
 */
void DmImageList::Attach(HIMAGELIST hIml)
{
	if (*this != hIml) {
		if (this->IsImageList()) {
			this->Release();
		}
		this->SetSelfHandle(hIml);
	}
}

/**
 *	@brief 開始拖曳影像
 *	@param[in] nImage		圖示索引碼
 *	@param[in] HotSpotPtr	指向結構 POINT 資料緩衝區位址，內容為甩曳開始位置。
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若操作失敗則返回值為零。
 */
BOOL DmImageList::BeginDrag(int nImage, POINT* HotSpotPtr) const
{
	if (this->IsImageList()) {
		return ::ImageList_BeginDrag(m_hIml, nImage, HotSpotPtr->x, HotSpotPtr->y);
	}
	return FALSE;
}

/**
 *	@brief 複製列表中圖示
 *	@param[in] iDst		複製目標圖示索引
 *	@param[in] iSrc		複製來源圖示索引
 *	@param[in] uFlags	複製方式旗標，預設 ILCF_MOVE
 */
BOOL DmImageList::Copy(int iDst, int iSrc, UINT uFlags) const
{
	if (this->IsImageList()) {
		return ::ImageList_Copy(*this, iDst, *this, iSrc, uFlags);
	}
	return FALSE;
}

/**
 *	@brief 刪除列表
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值，若運作失敗則返回值為零。
 */
BOOL DmImageList::DeleteImageList()
{
	if (this->IsImageList()) {
		return ::ImageList_Destroy(this->Detach());
	}
	return FALSE;
}

/**
 *	@brief 斷開連接
 *	@return <b>型別: HIMAGELIST</b> 返回已被斷開的 Image list 操作代碼。
 */
HIMAGELIST DmImageList::Detach()
{
	if (!this->IsImageList()) {
		return NULL;
	}

	HIMAGELIST hImageList = *this;
	this->SetSelfHandle(NULL);
	return hImageList;
}

/**
 *	@brief 釋放連接中的 Image list
 *	@return 此函數沒有返回值
 */
void DmImageList::Release()
{
	if (this->IsImageList()) {
		::ImageList_Destroy(*this);
		this->SetSelfHandle(NULL);
	}
}
