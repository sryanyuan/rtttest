#include "GameCommonDlg.h"
#include <hgeresource.h>
#include <Shlwapi.h>
//////////////////////////////////////////////////////////////////////////
unsigned int GetTextWidth(const char* _pText, size_t _nFontSize)
{
	size_t nLen = strlen(_pText);
	unsigned int nWidth = 0;

	for(size_t i = 0; i < nLen; ++i)
	{
		if(_pText[i] < 0)
		{
			//	The highest bit is '1', it is a chinese charactor
			nWidth += _nFontSize;
			++i;
		}
		else
		{
			nWidth += _nFontSize / 2;
		}
	}
	return nWidth;
}
//////////////////////////////////////////////////////////////////////////
hgeResourceManager* g_pResMgr = NULL;
static TextPrinter* printer = new TextPrinter;
static GfxFont* pFont12 = new GfxFont("宋体", 12, FALSE, FALSE, FALSE);
//(const char* lpsFontName, int nFaceSize, BOOL bBold /* = FALSE */, BOOL bItalic /* = FALSE */, BOOL bAntialias /* = TRUE */)
HGE* s_pHge = NULL;

TextPrinter* AfxGetPrinter()
{
	return printer;
}

HGE* AfxGetHge()
{
	return s_pHge;
}

void AfxSetHge(HGE* _pHge)
{
	s_pHge = _pHge;
}

GfxFont* AfxGetFont()
{
	return pFont12;
}
//////////////////////////////////////////////////////////////////////////
GameCommonDlg::GameCommonDlg()
{
	m_bLButtonDown = false;
	m_ptOrigin.x = m_ptOrigin.y = 0;
	ZeroMemory(&m_rcCloseBtn, sizeof(RECT));
	m_bCloseState = BTN_STATE_NORMAL;

	if(NULL == g_pResMgr)
	{
		char szBuf[MAX_PATH];
		GetModuleFileName(NULL, szBuf, sizeof(szBuf) - 1);
		PathRemoveFileSpec(szBuf);
		strcat(szBuf, "\\gameres\\game.dat");
		g_pResMgr = new hgeResourceManager(szBuf);
	}

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
}

GameCommonDlg::~GameCommonDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::ResetStatus()
{
	m_bCloseState = BTN_STATE_NORMAL;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType != CBTN_CHECKBOX)
		{
			begiter->bState = BTN_STATE_NORMAL;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::Render()
{
	if(m_rcCloseBtn.left == m_rcCloseBtn.right)
	{
		return;
	}

	HTEXTURE tex = 0;
	hgeResourceManager* pMgr = g_pResMgr;
	if(NULL == pMgr)
	{
		return;
	}

	tex = pMgr->GetTexture("close");
	if(tex &&
		m_pRender)
	{
		m_pRender->SetTexture(tex);
		if(m_bCloseState == BTN_STATE_NORMAL)
		{
			m_pRender->SetTextureRect(0, 0, 16, 16);
		}
		else if(m_bCloseState == BTN_STATE_OVER)
		{
			m_pRender->SetTextureRect(0, 16, 16, 16);
		}
		else if(m_bCloseState == BTN_STATE_DOWN)
		{
			m_pRender->SetTextureRect(0, 16*2, 16, 16);
		}
		m_pRender->Render(RELATIVE_X(m_rcCloseBtn.left), RELATIVE_Y(m_rcCloseBtn.top));
	}


	//	Normal buttons
	HTEXTURE texNormalBtn = pMgr->GetTexture("cnbtn");
	HTEXTURE texLeftArrowBtn = pMgr->GetTexture("clabtn");
	HTEXTURE texRightArrowBtn = pMgr->GetTexture("crabtn");
	//GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	bool bCanRender = false;

	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		bCanRender = false;

		if(begiter->nType == CBTN_NORMAL)
		{
			if(texNormalBtn != 0)
			{
				m_pRender->SetTexture(texNormalBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 21, 64, 21);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_LEFTARROW)
		{
			if(texLeftArrowBtn != 0)
			{
				m_pRender->SetTexture(texLeftArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_RIGHTARROW)
		{
			if(texRightArrowBtn)
			{
				m_pRender->SetTexture(texRightArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_CHECKBOX)
		{
			if(begiter->bState == BTN_STATE_UNCHECKED ||
				begiter->bState == BTN_STATE_CHECKED)
			{
				/*tex = pTexMgr->GetTexture(TEX_CHECKBOX + begiter->bState - BTN_STATE_UNCHECKED);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 17, 17);
					bCanRender = true;
				}*/
			}
		}
		else if(begiter->nType == CBTN_UPARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				/*tex = pTexMgr->GetTexture(TEX_UPARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				/*tex = pTexMgr->GetTexture(TEX_UPARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
		}
		else if(begiter->nType == CBTN_DOWNARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				/*tex = pTexMgr->GetTexture(TEX_DOWNARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				/*tex = pTexMgr->GetTexture(TEX_DOWNARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
		}

		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(begiter->stPos.left), RELATIVE_Y(begiter->stPos.top));

			if(strlen(begiter->szText))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + 20), RELATIVE_Y(begiter->stPos.top + 3),
						begiter->szText);
				}
				else
				{
					int nWidth = GetTextWidth(begiter->szText, 12);
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + (75 - nWidth) / 2), RELATIVE_Y(begiter->stPos.top + 9),
						begiter->szText);
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameCommonDlg::ProcUserCmd(const POINT& _mp)
{
	m_bCloseState = BTN_STATE_NORMAL;
	//ResetStatus();
	RECT rcTest;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType != CBTN_CHECKBOX)
		{
			begiter->bState = BTN_STATE_NORMAL;
		}
	}

	if(!IsCaptionBar(_mp.x, _mp.y) &&
		!m_bLButtonDown)
	{
		//m_bFocus = false;
		begiter = m_xButtons.begin();
		for(begiter; begiter != m_xButtons.end();
			++begiter)
		{
			//begiter->bState = BTN_STATE_NORMAL;

			rcTest.left = RELATIVE_X(begiter->stPos.left);
			rcTest.right = RELATIVE_X(begiter->stPos.right);
			rcTest.top = RELATIVE_Y(begiter->stPos.top);
			rcTest.bottom = RELATIVE_Y(begiter->stPos.bottom);
			if(PtInRect(&rcTest, _mp))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						BYTE bState = begiter->bState;
						if(bState == BTN_STATE_UNCHECKED)
						{
							bState = BTN_STATE_CHECKED;
						}
						else
						{
							bState = BTN_STATE_UNCHECKED;
						}
						begiter->bState = bState;
						OnCommonButtonClick(begiter->nID);
					}
				}
				else
				{
					begiter->bState = BTN_STATE_OVER;

					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_DOWN;
					}
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_OVER;
						OnCommonButtonClick(begiter->nID);
					}
				}
			}
		}

		return false;
	}

	rcTest.left = RELATIVE_X(m_rcCloseBtn.left);
	rcTest.right = RELATIVE_X(m_rcCloseBtn.right);
	rcTest.top = RELATIVE_Y(m_rcCloseBtn.top);
	rcTest.bottom = RELATIVE_Y(m_rcCloseBtn.bottom);

	if(PtInRect(&rcTest, _mp))
	{
		m_bCloseState = BTN_STATE_OVER;
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_bCloseState = BTN_STATE_DOWN;
		}
	}

	if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
	{
		m_ptOrigin.x = _mp.x;
		m_ptOrigin.y = _mp.y;
		m_bLButtonDown = true;
		//m_bFocus = true;
		//	提到顶层
		//::PostMessage(AfxGetHge()->System_GetState(HGE_HWND), WM_DLG_BRINGTOTOP, (WPARAM)this, 0);
		SetNeedTopLevel(true);
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		m_ptOrigin.x = m_ptOrigin.y = 0;
		m_bLButtonDown = false;

		if(m_bCloseState != BTN_STATE_NORMAL)
		{
			SetVisible(false);
		}
		//m_bFocus = false;
		return true;
	}

	
	if(m_bLButtonDown)
	{
		int nOffsetX = _mp.x - m_ptOrigin.x;
		int nOffsetY = _mp.y - m_ptOrigin.y;
		m_rcClient.left += nOffsetX;
		m_rcClient.right += nOffsetX;
		m_rcClient.top += nOffsetY;
		m_rcClient.bottom += nOffsetY;
		m_ptOrigin.x += nOffsetX;
		m_ptOrigin.y += nOffsetY;
	}

	return true;
}

bool GameCommonDlg::IsCaptionBar(int _x, int _y)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText /* = NULL */)
{
	if(_type < CBTN_NORMAL ||
		_type > CBTN_CHECKBOX)
	{
		return;
	}
	CommonButton stButton;
	stButton.bState = BTN_STATE_NORMAL;
	stButton.nType = _type;
	stButton.nID = _id;
	stButton.stPos.left = _x;
	stButton.stPos.top = _y;

	if(_type == CBTN_CHECKBOX)
	{
		stButton.bState = BTN_STATE_UNCHECKED;
	}

	if(_pszText == NULL)
	{
		stButton.szText[0] = 0;
	}
	else
	{
		if(strlen(_pszText) > 9)
		{
			return;
		}
		else
		{
			strcpy(stButton.szText, _pszText);
		}
	}
	if(_type == CBTN_NORMAL)
	{
		stButton.stPos.right = _x + 64;
		stButton.stPos.bottom = _y + 21;
	}
	else if(_type == CBTN_LEFTARROW ||
		_type == CBTN_RIGHTARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 15;
	}
	else if(_type == CBTN_CHECKBOX)
	{
		stButton.stPos.right = _x + 17;
		stButton.stPos.bottom = _y + 17;
	}
	else if(_type == CBTN_UPARROW ||
		_type == CBTN_DOWNARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 16;
	}
	m_xButtons.push_back(stButton);
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::RemoveCommonButton(int _id)
{
	int nSize = m_xButtons.size();
	if(nSize == 0)
	{
		return;
	}
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nID == _id)
		{
			m_xButtons.erase(begiter);
			return;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::MoveTo(int _x, int _y)
{
	int nOftX = _x - m_rcClient.left;
	int nOftY = _y - m_rcClient.top;

	m_rcClient.left += nOftX;
	m_rcClient.right += nOftX;
	m_rcClient.top += nOftY;
	m_rcClient.bottom += nOftY;
}

//////////////////////////////////////////////////////////////////////////
bool RenderDlgControl::ProcUserCmd(const POINT& _mp)
{
	//	需要反着遍历 不然消息处理不对 在链表最后一个的 应该第一个处理
	POINT ptMouse = _mp;
	bool bProcessed = false;
	RenderObject* pObj = NULL;
	RenderObject* pTopLevel = NULL;

	for(OBJECTLIST::reverse_iterator iter = m_objList.rbegin(); iter != m_objList.rend(); ++iter)
	{
		if(m_pStaticDlg)
		{
			//	Just process this
			if(!m_pStaticDlg->IsVisible())
			{
				m_pStaticDlg = NULL;
			}
			else
			{
				if(m_pStaticDlg->IsPtInObject(_mp) &&
					!bProcessed)
				{
					m_pStaticDlg->ProcUserCmd(_mp);
				}
				//	有静态窗口 总是返回已处理 堵塞游戏
				bProcessed = true;
			}
		}

		pObj = *iter;
		//	If it is static dialog
		if(pObj->IsStatic() &&
			pObj->IsVisible())
		{
			m_pStaticDlg = pObj;
		}

		if(!bProcessed)
		{
			if(pObj->IsVisible() &&
				pObj->IsPtInObject(ptMouse))
			{
				pObj->ProcUserCmd(_mp);
				bProcessed = true;
				/*if(pObj->IsNeedTopLevel())
				{
					pTopLevel = pObj;
				}*/
			}
			else if(pObj->IsVisible())
			{
				pObj->ResetStatus();
			}
		}
		else
		{
			if(pObj != m_pStaticDlg)
			{
				pObj->ResetStatus();
			}
		}

		if(pObj->IsVisible() &&
			pObj->IsNeedTopLevel())
		{
			pTopLevel = pObj;
		}
	}

	if(m_pStaticDlg)
	{
		BringToTop(m_pStaticDlg);
	}
	else
	{
		if(NULL != pTopLevel)
		{
			pTopLevel->SetNeedTopLevel(false);
			BringToTop(pTopLevel);
		}
	}
	return bProcessed;
}



//////////////////////////////////////////////////////////////////////////
GameNewDlg::GameNewDlg()
{
	m_nInfoWidth = m_nTitleWidth = 0;
	m_bCanMove = true;
	m_bCustomBk = false;
}

GameNewDlg::~GameNewDlg()
{

}

//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::CalcCloseButtonPosition()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return false;
	}

	m_rcCloseBtn.left = m_rcClient.right - m_rcClient.left - 43;
	m_rcCloseBtn.top = 0 + 9;
	m_rcCloseBtn.right = m_rcCloseBtn.left + 21;
	m_rcCloseBtn.bottom = 0 + 19 + 9;
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::IsCaptionBar(int _x, int _y)
{
	if(!m_bCanMove)
	{
		return false;
	}
	if(_y < RELATIVE_Y(30))
	{
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::ProcUserCmd(const POINT& _mp)
{
	m_bCloseState = BTN_STATE_NORMAL;
	//ResetStatus();
	RECT rcTest;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType != CBTN_CHECKBOX)
		{
			begiter->bState = BTN_STATE_NORMAL;
		}
	}

	if(!IsCaptionBar(_mp.x, _mp.y) &&
		!m_bLButtonDown)
	{
		//m_bFocus = false;
		begiter = m_xButtons.begin();
		for(begiter; begiter != m_xButtons.end();
			++begiter)
		{
			//begiter->bState = BTN_STATE_NORMAL;

			rcTest.left = RELATIVE_X(begiter->stPos.left);
			rcTest.right = RELATIVE_X(begiter->stPos.right);
			rcTest.top = RELATIVE_Y(begiter->stPos.top);
			rcTest.bottom = RELATIVE_Y(begiter->stPos.bottom);
			if(PtInRect(&rcTest, _mp))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						BYTE bState = begiter->bState;
						if(bState == BTN_STATE_UNCHECKED)
						{
							bState = BTN_STATE_CHECKED;
						}
						else
						{
							bState = BTN_STATE_UNCHECKED;
						}
						begiter->bState = bState;
						OnCommonButtonClick(begiter->nID);
					}
				}
				else
				{
					begiter->bState = BTN_STATE_OVER;

					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_DOWN;
					}
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_OVER;
						OnCommonButtonClick(begiter->nID);
					}
				}
			}
		}

		return false;
	}

	rcTest.left = RELATIVE_X(m_rcCloseBtn.left);
	rcTest.right = RELATIVE_X(m_rcCloseBtn.right);
	rcTest.top = RELATIVE_Y(m_rcCloseBtn.top);
	rcTest.bottom = RELATIVE_Y(m_rcCloseBtn.bottom);

	if(PtInRect(&rcTest, _mp))
	{
		m_bCloseState = BTN_STATE_OVER;
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_bCloseState = BTN_STATE_DOWN;
		}
	}

	if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
	{
		m_ptOrigin.x = _mp.x;
		m_ptOrigin.y = _mp.y;
		m_bLButtonDown = true;
		//m_bFocus = true;
		//	提到顶层
		//::PostMessage(AfxGetHge()->System_GetState(HGE_HWND), WM_DLG_BRINGTOTOP, (WPARAM)this, 0);
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		m_ptOrigin.x = m_ptOrigin.y = 0;
		if(m_bLButtonDown)
		{
			OnMoveStop();
		}
		m_bLButtonDown = false;

		if(m_bCloseState != BTN_STATE_NORMAL)
		{
			if(OnCloseDialog())
			{
				SetVisible(false);
			}
		}
		//m_bFocus = false;
		return true;
	}


	if(m_bLButtonDown)
	{
		int nOffsetX = _mp.x - m_ptOrigin.x;
		int nOffsetY = _mp.y - m_ptOrigin.y;
		m_rcClient.left += nOffsetX;
		m_rcClient.right += nOffsetX;
		m_rcClient.top += nOffsetY;
		m_rcClient.bottom += nOffsetY;
		m_ptOrigin.x += nOffsetX;
		m_ptOrigin.y += nOffsetY;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::ShowDialog()
{
	if(OnShowDialog())
	{
		m_bVisible = true;
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::CloseDialog()
{
	if(OnCloseDialog())
	{
		m_bVisible = false;
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::UpdateAlpha()
{
	DWORD dwColor = m_pRender->GetColor();
}
//////////////////////////////////////////////////////////////////////////
CommonButton* GameNewDlg::GetCommonButtonData(int _id)
{
	COMMONBUTTONS::iterator begIter = m_xButtons.begin();
	COMMONBUTTONS::const_iterator endIter = m_xButtons.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		if(begIter->nID == _id)
		{
			return &(*begIter);
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText )
{
	if(_type < CBTN_NORMAL ||
		_type > CBTN_CHECKBOX)
	{
		return;
	}
	CommonButton stButton;
	stButton.bState = BTN_STATE_NORMAL;
	stButton.nType = _type;
	stButton.nID = _id;
	stButton.stPos.left = _x;
	stButton.stPos.top = _y;

	if(_type == CBTN_CHECKBOX)
	{
		stButton.bState = BTN_STATE_UNCHECKED;
	}

	if(_pszText == NULL)
	{
		stButton.szText[0] = 0;
	}
	else
	{
		if(strlen(_pszText) > 9)
		{
			return;
		}
		else
		{
			strcpy(stButton.szText, _pszText);
		}
	}
	if(_type == CBTN_NORMAL)
	{
		stButton.stPos.right = _x + 75;
		stButton.stPos.bottom = _y + 30;
	}
	else if(_type == CBTN_LEFTARROW ||
		_type == CBTN_RIGHTARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 15;
	}
	else if(_type == CBTN_CHECKBOX)
	{
		stButton.stPos.right = _x + 17;
		stButton.stPos.bottom = _y + 17;
	}
	else if(_type == CBTN_UPARROW ||
		_type == CBTN_DOWNARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 16;
	}

	m_xButtons.push_back(stButton);
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::Render()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return;
	}

	//hgeResourceManager* pMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	hgeResourceManager* pMgr = g_pResMgr;
	if(NULL == pMgr)
	{
		return;
	}
	if(NULL == m_pRender)
	{
		return;
	}

	bool bCanRender = false;
	HTEXTURE tex = 0;

	if(!m_bCustomBk)
	{
		//	Draw title
		tex = pMgr->GetTexture("ndlglt");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 97, 72);
			m_pRender->Render(m_rcClient.left, m_rcClient.top);
		}
		//	Title border
		int nTileBorderWidth = (int)m_rcClient.right - (int)m_rcClient.left - 97 - 96;
		tex = pMgr->GetTexture("ndlgct");
		if(tex &&
			nTileBorderWidth > 0)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, nTileBorderWidth, 69);
			m_pRender->Render(m_rcClient.left + 97, m_rcClient.top + 3);
		}
		tex = pMgr->GetTexture("ndlgrt");
		if(tex &&
			nTileBorderWidth > 0)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 96, 73);
			m_pRender->Render(m_rcClient.left + 97 + nTileBorderWidth, m_rcClient.top);
		}

		//	Bk
		tex = pMgr->GetTexture("ndlgbk");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, m_rcClient.right - m_rcClient.left - 18, m_rcClient.bottom - m_rcClient.top - 72 - 16 + 1);
			m_pRender->Render(m_rcClient.left + 9, m_rcClient.top + 72);
		}

		//	Bord
		tex = pMgr->GetTexture("ndlglb");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 10, m_rcClient.bottom - m_rcClient.top - 72 - 16);
			m_pRender->Render(m_rcClient.left, m_rcClient.top + 72);
		}
		tex = pMgr->GetTexture("ndlgrb");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 10, m_rcClient.bottom - m_rcClient.top - 72 - 16);
			m_pRender->Render(m_rcClient.right - 10, m_rcClient.top + 72);
		}

		//	Down border
		tex = pMgr->GetTexture("ndlglbn");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 55, 50);
			m_pRender->Render(m_rcClient.left, m_rcClient.bottom - 50);
		}
		tex = pMgr->GetTexture("ndlgrbn");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 54, 50);
			m_pRender->Render(m_rcClient.right - 54, m_rcClient.bottom - 50);
		}
		tex = pMgr->GetTexture("ndlgdb");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, m_rcClient.right - m_rcClient.left - 54 - 55 + 6, 12);
			m_pRender->Render(m_rcClient.left + 50, m_rcClient.bottom - 15);
		}

		tex = pMgr->GetTexture("ndlgclose");
		if(tex &&
			m_pRender)
		{
			m_pRender->SetTexture(tex);
			if(m_bCloseState == BTN_STATE_NORMAL)
			{
				//m_pRender->SetTextureRect(0, 0, 21, 19);
			}
			else if(m_bCloseState == BTN_STATE_OVER)
			{
				m_pRender->SetTextureRect(0, 0, 21, 19);
				bCanRender = true;
			}
			else if(m_bCloseState == BTN_STATE_DOWN)
			{
				m_pRender->SetTextureRect(0, 19 * 1, 21, 19);
				bCanRender = true;
			}
			if(bCanRender)
			{
				m_pRender->Render(RELATIVE_X(m_rcCloseBtn.left), RELATIVE_Y(m_rcCloseBtn.top));
			}
		}

		//	Title
		if(!m_xTitle.empty())
		{
			int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nTitleWidth) / 2;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(12), m_xTitle.c_str());
		}
		//	Info
		if(!m_xInfo.empty())
		{
			int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nInfoWidth) / 2;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(43), m_xInfo.c_str());
		}
	}

	//	Normal buttons
	//HTEXTURE texNormalBtn = pMgr->GetTexture("cnbtn");
	HTEXTURE texNormalBtn = pMgr->GetTexture("sysbtn");
	HTEXTURE texLeftArrowBtn = pMgr->GetTexture("clabtn");
	HTEXTURE texRightArrowBtn = pMgr->GetTexture("crabtn");
	//GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	bCanRender = false;

	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		bCanRender = false;

		if(begiter->nType == CBTN_NORMAL)
		{
#ifdef _DEBUG
			if(begiter->bState != BTN_STATE_NORMAL)
			{
				//DEBUG_BREAK;
			}
#endif
			if(texNormalBtn != 0)
			{
				m_pRender->SetTexture(texNormalBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 30, 75, 30);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_LEFTARROW)
		{
			if(texLeftArrowBtn != 0)
			{
				m_pRender->SetTexture(texLeftArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_RIGHTARROW)
		{
			if(texRightArrowBtn)
			{
				m_pRender->SetTexture(texRightArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_CHECKBOX)
		{
			if(begiter->bState == BTN_STATE_UNCHECKED ||
				begiter->bState == BTN_STATE_CHECKED)
			{
				/*tex = pTexMgr->GetTexture(TEX_CHECKBOX + begiter->bState - BTN_STATE_UNCHECKED);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 17, 17);
					bCanRender = true;
				}*/
			}
		}
		else if(begiter->nType == CBTN_UPARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				/*tex = pTexMgr->GetTexture(TEX_UPARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				/*tex = pTexMgr->GetTexture(TEX_UPARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
		}
		else if(begiter->nType == CBTN_DOWNARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				/*tex = pTexMgr->GetTexture(TEX_DOWNARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				/*tex = pTexMgr->GetTexture(TEX_DOWNARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}*/
			}
		}

		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(begiter->stPos.left), RELATIVE_Y(begiter->stPos.top));

			if(strlen(begiter->szText))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + 20), RELATIVE_Y(begiter->stPos.top + 3),
						begiter->szText);
				}
				else
				{
					int nWidth = GetTextWidth(begiter->szText, 12);
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + (75 - nWidth) / 2), RELATIVE_Y(begiter->stPos.top + 9),
						begiter->szText);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////