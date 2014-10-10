#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
#define COLOR_SUITNAME			ARGB(255, 178, 234, 97)
#define COLOR_SUITNAMEITEM_ACTIVE	ARGB(255, 152, 199, 146)
#define COLOR_SUITNAMEITEM_UNACTIVE	ARGB(255, 135, 135, 135)
#define COLOR_EXTRAATRRIB_ACTIVE	ARGB(255, 103, 224, 246)
#define COLOR_EXTRAATTRIB_UNACTIVE	ARGB(255, 135, 135, 135)
//////////////////////////////////////////////////////////////////////////
static const DWORD s_dwItemNameColor = ARGB(255, 195, 119, 69);

static const int s_nTitleHeight = 16;
static const int s_nTopBoundary = 5;
static const int s_nLeftBoundary = 5;
static const int s_nEachLineHeight = 13;
static const int s_nItemIconWidth = 48;
static const int s_nItemIconHeight = 48;
static const int s_nItemIntroGapX = 5;

static const char* s_pszItemLevel[] =
{
	"��ͨ",
	"��Ʒ",
	"����",
	"��Ͼ",
	"����",
	"׿Խ",
	"�Ƿ�",
	"��˵",
	"ʷʫ"
};
//////////////////////////////////////////////////////////////////////////
int GetNumberBit(int _number)
{
	int nMaxBit = 8;

	for(int i = 1; i <= nMaxBit; ++i)
	{
		if(_number < pow((double)10, i))
		{
			return i;
		}
	}
	return 0;
}

bool IsEquipItem(const ItemAttrib* _pItem)
{
	return _pItem->type == 1;
	return true;
}
//////////////////////////////////////////////////////////////////////////
GameDisplayDlg::GameDisplayDlg()
{
	SetCustomBk();

	m_nScreenHeight = 600;
	m_nScreenWidth = 800;

	ResetItemAttirb();
}

GameDisplayDlg::~GameDisplayDlg()
{

}

void GameDisplayDlg::ResetItemAttirb()
{
	ZeroMemory(m_stItems, sizeof(m_stItems));
	ZeroMemory(&m_stShowItem, sizeof(ItemAttrib));
	m_nOwner = 0;
}

void GameDisplayDlg::ClearItems()
{
	if(m_xItemList.empty())
	{
		return;
	}

	std::list<DisplayItem*>::const_iterator begIter = m_xItemList.begin();
	std::list<DisplayItem*>::const_iterator endIter = m_xItemList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		DisplayItem* pDisplayItem = *begIter;
		m_xItemFactory.PushFreePtr(pDisplayItem);
	}

	m_xItemList.clear();
}

DisplayItem* GameDisplayDlg::AddStringItem(const char* _pszText, int _nx, int _ny, DWORD _dwColor /* = 0 */, GfxFont* _pFont /* = NULL */)
{
	DisplayItem* pItem = m_xItemFactory.PopFreePtr();
	pItem->nShowType = SHOW_STRING;
	pItem->xText = _pszText;
	pItem->nPosX = _nx;
	pItem->nPosY = _ny;
	pItem->dwColor = _dwColor;
	pItem->pFont = _pFont;

	m_xItemList.push_back(pItem);
	return pItem;
}

void GameDisplayDlg::Render()
{
	if(m_xItemList.empty())
	{
		return;
	}
	if(m_rcClient.left == m_rcClient.right ||
		m_rcClient.top == m_rcClient.bottom)
	{
		return;
	}

	hgeResourceManager* pResMgr = g_pResMgr;
	HTEXTURE tex = 0;

	{
		//	draw bk
		tex = pResMgr->GetTexture("bmcolor");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0,
				0,
				m_rcClient.right - m_rcClient.left,
				m_rcClient.bottom - m_rcClient.top);
			m_pRender->Render(m_rcClient.left,
				m_rcClient.top);
		}
	}

	int nDrawX = 0;
	int nDrawY = 0;

	{
		//	render strings
		std::list<DisplayItem*>::const_iterator begIter = m_xItemList.begin();
		std::list<DisplayItem*>::const_iterator endIter = m_xItemList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			DisplayItem* pDisplayItem = *begIter;

			if(pDisplayItem)
			{
				GfxFont* pFont = pDisplayItem->pFont;

				if(SHOW_STRING == pDisplayItem->nShowType)
				{
					if(NULL == pFont)
					{
						//	default font
						pFont = AfxGetFont();
					}

					pFont->SetColor(ARGB_WHITE);
					if(0 != pDisplayItem->dwColor)
					{
						pFont->SetColor(pDisplayItem->dwColor);
					}

					nDrawX = RELATIVE_X(pDisplayItem->nPosX);
					nDrawY = RELATIVE_Y(pDisplayItem->nPosY);

					pFont->Print((float)nDrawX,
						(float)nDrawY,
						pDisplayItem->xText.c_str());
				}
			}
		}
	}
}

bool GameDisplayDlg::ProcUserCmd(const POINT& _mp)
{
	int nShowX = _mp.x;
	int nShowY = _mp.y;
	int nClientWidth = m_rcClient.right - m_rcClient.left;
	int nClientHeight = m_rcClient.bottom - m_rcClient.top;
	nShowX += 5;
	nShowY += 5;

	if(nShowX + nClientWidth > m_nScreenWidth)
	{
		int nOffset = nShowX + nClientWidth - m_nScreenWidth;
		nShowX -= nOffset;
	}
	if(nShowY + nClientHeight > m_nScreenHeight)
	{
		int nOffset = nShowY + nClientHeight - m_nScreenHeight;
		nShowY -= nOffset;
	}
	MoveTo(nShowX, nShowY);

	return true;
}

void GameDisplayDlg::ShowItemAttrib(const ItemAttrib* _pItem, int _nOwner /* = -1 */)
{
	//	��ʾ������װ�� ��Ҫ��ǰ����������ұ��װ��
	int nDestOwner = _nOwner;

	if(-1 == nDestOwner)
	{
		//	(GamePlayer::GetInstance())
	}

	//	�ж���Ʒtag, tag��ͬ��������ʾ�б�
	if(m_stShowItem.tag != 0)
	{
		if(m_stShowItem.tag == _pItem->tag)
		{
			ShowDialog();
			return;
		}
	}

	memcpy(&m_stShowItem, _pItem, sizeof(ItemAttrib));

	GenShowItems();
	ShowDialog();
}

void GameDisplayDlg::GenShowItems()
{
	ClearItems();

	if(m_stShowItem.type == 0)
	{
		return;
	}
	//	Reset the original item attribute
	ItemAttrib stOriItem;
	ZeroMemory(&stOriItem, sizeof(ItemAttrib));
	if(IsEquipItem(&m_stShowItem))
	{
		stOriItem.id = m_stShowItem.id;
		//if(GameInfoManager::GetInstance()->GetItemAttrib(m_pItemAttrib->id, &m_oriItem))
		{
			//	Calculate the addition of the equipment
#define GET_ITEM_ADDITION(ATTRIB)	stOriItem.ATTRIB = m_stShowItem.ATTRIB - stOriItem.ATTRIB;
			GET_ITEM_ADDITION(maxAC);
			GET_ITEM_ADDITION(maxMAC);
			GET_ITEM_ADDITION(maxDC);
			GET_ITEM_ADDITION(maxMC);
			GET_ITEM_ADDITION(maxSC);
			GET_ITEM_ADDITION(lucky);
			GET_ITEM_ADDITION(atkSpeed);
			GET_ITEM_ADDITION(moveSpeed);
			GET_ITEM_ADDITION(accuracy);
			GET_ITEM_ADDITION(hide);
		}
		//else
		{
			//ZeroMemory(&m_oriItem, sizeof(ItemAttrib));
		}
	}

	//	start
	//int nRectHeight = 0;
	int nMaxWidth = 0;
	int nCurWidth = 0;

#define TEST_MAXWIDTH(width)				{if(width > nMaxWidth){nMaxWidth = width;}};
#define TEST_TEXTWIDTH(text, fsize)			{int nTextLength = GetTextWidth(text, fsize);TEST_MAXWIDTH(nTextLength);}

	int nDrawRectX = s_nLeftBoundary;
	int nDrawRectY = s_nTopBoundary;

	int nCurX = nDrawRectX;
	int nCurY = nDrawRectY;

	char szText[MAX_PATH];
	//	first, quality
	int nQualityIndex = m_stShowItem.level;
	if(nQualityIndex < 0 ||
		nQualityIndex >= sizeof(s_pszItemLevel) / sizeof(s_pszItemLevel[0]))
	{
		nQualityIndex = 0;
	}

	//	name
	TEST_TEXTWIDTH(m_stShowItem.name, 14);
	DisplayItem* pNameItem = AddStringItem(m_stShowItem.name, nCurX, nCurY, ARGB_YELLOW, AfxGetFont14());
	//nRectHeight += s_nTitleHeight;

	//	item icon
	nCurX = nDrawRectX;
	nCurY = nDrawRectY;
	nCurY += s_nTitleHeight;
	//nRectHeight += s_nItemIconHeight;

	int nIntroLine = 0;
	//	type
	sprintf(szText, "���ͣ�%s", "�鼮");
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetTextWidth(szText, 12);
	TEST_MAXWIDTH(nCurWidth);
	nCurX = nDrawRectX;
	nCurY = nDrawRectY;
	nCurX += s_nItemIconWidth + s_nItemIntroGapX;
	nCurY += s_nTitleHeight + 5 + nIntroLine * s_nEachLineHeight;
	AddStringItem(szText, nCurX, nCurY);
	++nIntroLine;

	//	weight
	sprintf(szText, "������%d", m_stShowItem.weight);
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetTextWidth(szText, 12);
	TEST_MAXWIDTH(nCurWidth);
	nCurX = nDrawRectX;
	nCurY = nDrawRectY;
	nCurX += s_nItemIconWidth + s_nItemIntroGapX;
	nCurY += s_nTitleHeight + 5 + nIntroLine * s_nEachLineHeight;
	AddStringItem(szText, nCurX, nCurY);
	++nIntroLine;

	//	bind or not
	sprintf(szText, "%s", TEST_FLAG_BOOL(m_stShowItem.atkPois, 0x1) ? "[�Ѱ�]" : "[δ��]");
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetTextWidth(szText, 12);
	TEST_MAXWIDTH(nCurWidth);
	nCurX = nDrawRectX;
	nCurY = nDrawRectY;
	nCurX += s_nItemIconWidth + s_nItemIntroGapX;
	nCurY += s_nTitleHeight + 5 + nIntroLine * s_nEachLineHeight;
	AddStringItem(szText, nCurX, nCurY);
	++nIntroLine;

	//	details
	int nDetailLine = 0;
	int nDetailDrawX = s_nLeftBoundary;
	int nDetailDrawY = s_nTopBoundary + s_nTitleHeight + s_nItemIconHeight + s_nItemIntroGapX;

	if(IsEquipItem(&m_stShowItem))
	{
		//	*equipment*
		//	equipment information
		nCurX = nDetailDrawX;
		nCurY = nDetailDrawY + nDetailLine * s_nEachLineHeight;
		nCurWidth = 5 * 12;
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem("[װ����Ϣ]", nCurX, nCurY);
		++nDetailLine;

		//	dura
		nCurX = nDetailDrawX;
		nCurY = nDetailDrawY + nDetailLine * s_nEachLineHeight;
		int nDura = HIWORD(m_stShowItem.maxHP) / 1000;
		int nDuraMax = LOWORD(m_stShowItem.maxHP) / 1000;
		sprintf(szText, "�־ã�%d/%d", nDura, nDuraMax);
		nCurWidth = GetTextWidth(szText, 12);
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem(szText, nCurX, nCurY);
		++nDetailLine;

		//	quality
		nCurX = nDrawRectX;
		nCurY = nDetailDrawY + nDetailLine * s_nEachLineHeight;
		sprintf(szText, "Ʒ�ʣ�%s", s_pszItemLevel[nQualityIndex]);
		nCurWidth = GetTextWidth(szText, 12);
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem(szText, nCurX, nCurY);
		++nDetailLine;

		//	level
		int nLevel = 0;
		nCurX = nDrawRectX;
		nCurY = nDetailDrawY + nDetailLine * s_nEachLineHeight;
		sprintf(szText, "����%d", nLevel);
		nCurWidth = GetTextWidth(szText, 12);
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem(szText, nCurX, nCurY);
		++nDetailLine;

		//	a blank line
		++nDetailLine;

		//	equipment base attribute
		nCurX = nDetailDrawX;
		nCurY = nDetailDrawY + nDetailLine * s_nEachLineHeight;
		nCurWidth = 5 * 12;
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem("[��������]", nCurX, nCurY);
		++nDetailLine;

		//	AC
		if(m_stShowItem.AC != 0 ||
			m_stShowItem.maxAC != 0)
		{
			if(m_stShowItem.type != /*ITEM_WEAPON*/111 &&
				m_stShowItem.type != /*ITEM_NECKLACE*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				if(stOriItem.maxAC != 0)
				{
					sprintf(szText, "������%d-%d(+%d)",
						m_stShowItem.AC, m_stShowItem.maxAC, stOriItem.maxAC);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "������%d-%d",
						m_stShowItem.AC, m_stShowItem.maxAC);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	MAC
		if(m_stShowItem.MAC != 0 ||
			m_stShowItem.maxMAC != 0)
		{
			if(m_stShowItem.type != /*ITEM_WEAPON*/111 &&
				m_stShowItem.type != /*ITEM_NECKLACE*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				if(stOriItem.maxAC != 0)
				{
					sprintf(szText, "ħ����%d-%d(+%d)",
						m_stShowItem.MAC, m_stShowItem.maxMAC, stOriItem.maxMAC);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "ħ����%d-%d",
						m_stShowItem.MAC, m_stShowItem.maxMAC);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	DC
		if(m_stShowItem.DC != 0 ||
			m_stShowItem.maxDC != 0)
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			if(stOriItem.maxDC != 0)
			{
				sprintf(szText, "������%d-%d(+%d)",
					m_stShowItem.DC, m_stShowItem.maxDC, stOriItem.maxDC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
			}
			else
			{
				sprintf(szText, "������%d-%d",
					m_stShowItem.DC, m_stShowItem.maxDC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY);
			}

			++nDetailLine;
		}

		//	MC
		if(m_stShowItem.MC != 0 ||
			m_stShowItem.maxMC != 0)
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			if(stOriItem.maxMC != 0)
			{
				sprintf(szText, "ħ����%d-%d(+%d)",
					m_stShowItem.MC, m_stShowItem.maxMC, stOriItem.maxMC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
			}
			else
			{
				sprintf(szText, "ħ����%d-%d",
					m_stShowItem.MC, m_stShowItem.maxMC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY);
			}

			++nDetailLine;
		}

		//	SC
		if(m_stShowItem.SC != 0 ||
			m_stShowItem.maxSC != 0)
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			if(stOriItem.maxSC != 0)
			{
				sprintf(szText, "������%d-%d(+%d)",
					m_stShowItem.SC, m_stShowItem.maxSC, stOriItem.maxSC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
			}
			else
			{
				sprintf(szText, "������%d-%d",
					m_stShowItem.SC, m_stShowItem.maxSC);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY);
			}

			++nDetailLine;
		}

		//	Lucky
		if(m_stShowItem.lucky != 0)
		{
			if(m_stShowItem.type == /*ITEM_WEAPON*/111 ||
				m_stShowItem.type == /*ITEM_NECKLACE*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(stOriItem.lucky != 0)
				{
					sprintf(szText, "���ˣ�+%d(+%d)",
						m_stShowItem.lucky, stOriItem.lucky);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "���ˣ�+%d",
						m_stShowItem.lucky);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	AtkSpeed
		if(m_stShowItem.atkSpeed != 0)
		{
			if(m_stShowItem.type == /*ITEM_RING*/111 ||
				m_stShowItem.type == /*ITEM_NECKLACE*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(stOriItem.atkSpeed != 0)
				{
					sprintf(szText, "�����ٶȣ�+%d(+%d)",
						m_stShowItem.atkSpeed, stOriItem.atkSpeed);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "�����ٶȣ�+%d",
						m_stShowItem.atkSpeed);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	MoveSpeed
		if(m_stShowItem.moveSpeed != 0)
		{
			//if(m_pItemAttrib->type == ITEM_RING ||
			//m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(stOriItem.moveSpeed != 0)
				{
					sprintf(szText, "�ƶ��ٶȣ�+%d(+%d)",
						m_stShowItem.moveSpeed, stOriItem.moveSpeed);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "�ƶ��ٶȣ�+%d",
						m_stShowItem.moveSpeed);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	Accuracy
		if(m_stShowItem.accuracy != 0)
		{
			if(m_stShowItem.type == /*ITEM_BRACELAT*/111 ||
				m_stShowItem.type == /*ITEM_NECKLACE*/111 ||
				m_stShowItem.type == /*ITEM_WEAPON*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(stOriItem.accuracy != 0)
				{
					sprintf(szText, "׼ȷ��+%d(+%d)",
						m_stShowItem.accuracy, stOriItem.accuracy);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "׼ȷ��+%d",
						m_stShowItem.accuracy);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}

		//	Hide
		if(m_stShowItem.hide != 0)
		{
			if(m_stShowItem.type == /*ITEM_BRACELAT*/111 ||
				m_stShowItem.type == /*ITEM_NECKLACE*/111)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(stOriItem.hide != 0)
				{
					sprintf(szText, "��ܣ�+%d(+%d)",
						m_stShowItem.hide, stOriItem.hide);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
				}
				else
				{
					sprintf(szText, "��ܣ�+%d",
						m_stShowItem.hide);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}

				++nDetailLine;
			}
		}
	}
	else if(m_stShowItem.type == /*ITEM_COST*/1)
	{
		//	[��Ʒ��Ϣ]
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			sprintf(szText, "[��Ʒ��Ϣ]");
			nCurWidth = 12 * 5;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem(szText, nCurX, nCurY);
			++nDetailLine;
		}
		//	Count
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			sprintf(szText, "������%d",
				m_stShowItem.atkSpeed);
			nCurWidth = GetTextWidth(szText, 12);
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem(szText, nCurX, nCurY);
			++nDetailLine;
		}

		//	Drug
		if(m_stShowItem.curse == 0)
		{
			//	HP
			if(m_stShowItem.HP != 0)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				if(m_stShowItem.lucky == 0)
				{
					sprintf(szText, "��������HP��%d",
						m_stShowItem.HP);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}
				else if(m_stShowItem.lucky == 1)
				{
					sprintf(szText, "���ٲ���HP��%d",
						m_stShowItem.HP);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}
				++nDetailLine;
			}
			//	MP
			if(m_stShowItem.MP != 0)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				if(m_stShowItem.lucky == 0)
				{
					sprintf(szText, "��������MP��%d",
						m_stShowItem.MP);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}
				else if(m_stShowItem.lucky == 1)
				{
					sprintf(szText, "���ٲ���MP��%d",
						m_stShowItem.MP);
					nCurWidth = GetTextWidth(szText, 12);
					TEST_MAXWIDTH(nCurWidth);
					AddStringItem(szText, nCurX, nCurY);
				}
				++nDetailLine;
			}
			//	Cooldown time
			if(m_stShowItem.hide != 0)
			{
				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;
				sprintf(szText, "��ȴʱ�䣺%d��",
					m_stShowItem.hide);
				nCurWidth = GetTextWidth(szText, 12);
				TEST_MAXWIDTH(nCurWidth);
				AddStringItem(szText, nCurX, nCurY);
				++nDetailLine;
			}
		}
	}
	else if(m_stShowItem.type == /*ITEM_BOOK*/2)
	{
		//	[�ؼ���Ϣ]
		nCurX = nDetailDrawX;
		nCurY = nDetailDrawY;
		nCurY += s_nEachLineHeight * nDetailLine;
		nCurWidth = 12 * 5;
		TEST_MAXWIDTH(nCurWidth);
		AddStringItem("[�ؼ���Ϣ]", nCurX, nCurY);
		++nDetailLine;

		//	Job
		nCurX = nDetailDrawX;
		nCurY = nDetailDrawY;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		if(m_stShowItem.lucky == 1)
		{
			nCurWidth = 12 * 4;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("սʿ�ؼ�", nCurX, nCurY);
		}
		else if(m_stShowItem.lucky == 2)
		{
			nCurWidth = 12 * 4;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("��ʦ�ؼ�", nCurX, nCurY);
		}
		else if(m_stShowItem.lucky == 3)
		{
			nCurWidth = 12 * 4;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("��ʿ�ؼ�", nCurX, nCurY);
		}
		else
		{
			nCurWidth = 12 * 4;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("������", nCurX, nCurY);
		}

		++nDetailLine;
	}

	//	suit attribute
	/*if(IsEquipItem(&m_stShowItem) &&
		m_stShowItem.atkPalsy != 0)
	{
		SuitExtraAttrib* pExtraAttribList = GetGlobalSuitExtraAttrib(m_stShowItem.atkPalsy);

		if(pExtraAttribList)
		{
			//	a line blank
			++nDetailLine;

			//	Suit name
			const ItemAttrib* pPlayerItem = NULL;
			bool bActived[PLAYER_ITEM_TOTAL];
			ZeroMemory(bActived, sizeof(bActived));
			int nActiveItemSum = 0;
			int nActiveItemAll = 0;

			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if(pExtraAttribList->nSuitEquipID[i] != 0)
				{
					++nActiveItemAll;
				}
			}

			for(int i = PLAYER_ITEM_WEAPON; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if(-1 == m_nOwner)
				{
					pPlayerItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);
				}
				else
				{
					pPlayerItem = m_stItems[i];
				}

				if(pPlayerItem->atkPalsy == pExtraAttribList->nSuitID)
				{
					//	����Ƿ���ڹ�
					for(int j = 0; j < 10; ++j)
					{
						if(pPlayerItem->id == pExtraAttribList->nSuitEquipID[j] &&
							!bActived[j] &&
							pExtraAttribList->nSuitEquipID[j] != 0)
						{
							bActived[j] = true;
							++nActiveItemSum;
							break;
						}
					}
				}
			}

			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			sprintf(szText, "[%s](%d/%d)",
				pExtraAttribList->szSuitChName,
				nActiveItemSum,
				nActiveItemAll);
			nCurWidth = GetTextWidth(szText, 12);
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem(szText, nCurX, nCurY, ARGB_GREEN);
			++nDetailLine;
			
			//	suit item name
			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if(pExtraAttribList->nSuitEquipID[i] != 0)
				{
					ItemAttrib item;
					ZeroMemory(&item, sizeof(ItemAttrib));

					if(GameInfoManager::GetInstance()->GetItemAttrib(pExtraAttribList->nSuitEquipID[i], &item))
					{
						nCurX = nDetailDrawX;
						nCurY = nDetailDrawY;
						nCurY += s_nEachLineHeight * nDetailLine;
						nCurWidth = GetTextWidth(item.name, 12);
						TEST_MAXWIDTH(nCurWidth);
						if(bActived[i])
						{
							AddStringItem(item.name, nCurX, nCurY, COLOR_SUITNAMEITEM_ACTIVE);
						}
						else
						{
							AddStringItem(item.name, nCurX, nCurY, COLOR_SUITNAMEITEM_UNACTIVE);
						}
						
						++nDetailLine;
					}
				}
			}

			//	a line blank
			++nDetailLine;

			//	suit effect describe
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_GREEN);
			if(pExtraAttribList->nSuitShowType == 1)
			{
				AddStringItem("[����Ч��]", nCurX, nCurY, ARGB_GREEN);
			}
			else
			{
				AddStringItem("[��װЧ��]", nCurX, nCurY, ARGB_GREEN);
			}
			++nDetailLine;

			//	suit effect
			int nActiveMax = 0;
			int nActiveAttribSum = 0;

			for(int i = 0; i < MAX_EXTRAATTIRB; ++i)
			{
				if(nActiveItemSum >= pExtraAttribList->nActiveSum[i] &&
					pExtraAttribList->nActiveSum[i] > nActiveMax)
				{
					nActiveMax = pExtraAttribList->nActiveSum[i];
					nActiveAttribSum = pExtraAttribList->nActiveAttribSum[i];
				}
			}

			ItemExtraAttribItem extraAttrib;

			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if(pExtraAttribList->stExtraAttrib[i].nAttribID == 0)
				{
					break;
				}

				DWORD dwItemColor = COLOR_EXTRAATTRIB_UNACTIVE;
				if(nActiveAttribSum > i)
				{
					dwItemColor = COLOR_EXTRAATRRIB_ACTIVE;
				}

				extraItem = pExtraAttribList->stExtraAttrib[i];
				if(extraItem.nAttribID == EAID_AC ||
					extraItem.nAttribID == EAID_MAC ||
					extraItem.nAttribID == EAID_DC ||
					extraItem.nAttribID == EAID_MC ||
					extraItem.nAttribID == EAID_SC)
				{
					sprintf(szText, "%s%d-%d",
						g_szExtraAttribDescriptor[extraItem.nAttribID], HIWORD(extraItem.nAttribValue), LOWORD(extraItem.nAttribValue));
				}
				else
				{
					sprintf(szText, "%s+%d",
						g_szExtraAttribDescriptor[extraItem.nAttribID], extraItem.nAttribValue);
				}

				nCurX = nDetailDrawX;
				nCurY = nDetailDrawY;
				nCurY += s_nEachLineHeight * nDetailLine;

				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, szText, dwItemColor);
				++nDetailLine;
			}
		}
	}*/

	//	addition information
	if(TEST_FLAG_BOOL(m_stShowItem.EXPR, /*EXPR_MASK_NOSAVE*/1) ||
		TEST_FLAG_BOOL(m_stShowItem.EXPR, /*EXPR_MASK_NOSELL*/2))
	{
		//	A line blank
		++nDetailLine;

		//	No save
		if(TEST_FLAG_BOOL(m_stShowItem.EXPR, /*EXPR_MASK_NOSAVE*/1))
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			nCurWidth = 12 * 6;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("���޷����棩", nCurX, nCurY, ARGB_RED);
			++nDetailLine;
		}
		if(TEST_FLAG_BOOL(m_stShowItem.EXPR, /*EXPR_MASK_NOSELL*/2))
		{
			nCurX = nDetailDrawX;
			nCurY = nDetailDrawY;
			nCurY += s_nEachLineHeight * nDetailLine;
			nCurWidth = 12 * 6;
			TEST_MAXWIDTH(nCurWidth);
			AddStringItem("���޷����ۣ�", nCurX, nCurY, ARGB_RED);
			++nDetailLine;
		}
	}

	//	end, add border width
	m_rcClient.left = m_rcClient.top = 0;
	m_rcClient.right = nMaxWidth + 2 * s_nLeftBoundary;
	m_rcClient.bottom = s_nTitleHeight + s_nItemIconHeight + s_nItemIntroGapX + s_nEachLineHeight * nDetailLine + 2 * s_nTopBoundary;

	//	adjust name rect
	if(pNameItem)
	{
		int nNameCenterX = GetTextWidth(pNameItem->xText.c_str(), 14);
		nNameCenterX = (m_rcClient.right - m_rcClient.left - nNameCenterX) / 2;
		pNameItem->nPosX = nNameCenterX;
	}
}