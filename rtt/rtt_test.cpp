#include "rtt_test.h"
#include <stdio.h>
#include <Shlwapi.h>
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////////
int g_nScreenWidth = 0;
int g_nScreenHeight = 0;
//////////////////////////////////////////////////////////////////////////
const char* GetFullPath(const char* _pszSubFile)
{
	static char szPath[MAX_PATH] = {0};
	if(0 == szPath[0])
	{
		GetModuleFileName(NULL, szPath, sizeof(szPath));
		PathRemoveFileSpec(szPath);
	}
	
	static char szBuf[MAX_PATH];
	sprintf(szBuf, "%s/%s",
		szPath, _pszSubFile);

	return szBuf;
}
//////////////////////////////////////////////////////////////////////////
inline BYTE MakeByte(BYTE _bLow, BYTE _bHigh)
{
	return (BYTE)((_bLow & 0x0f) | ((_bHigh & 0xff) << 4));
}

inline BYTE GetLowHalfByte(BYTE _bData)
{
	return (_bData & 0x0f);
}

inline BYTE GetHighHalfByte(BYTE _bData)
{
	return ((_bData & 0xf0) >> 4);
}

DWORD MakeDwordWithHalfBytes(BYTE _bLow0, BYTE _bHigh0, BYTE _bLow1, BYTE _bHigh1, BYTE _bLow2, BYTE _bHigh2, BYTE _bLow3, BYTE _bHigh3)
{
	BYTE bByte0 = MakeByte(_bLow0, _bHigh0);
	BYTE bByte1 = MakeByte(_bLow1, _bHigh1);
	BYTE bByte2 = MakeByte(_bLow2, _bHigh2);
	BYTE bByte3 = MakeByte(_bLow3, _bHigh3);

	WORD wLow = MAKEWORD(bByte0, bByte1);
	WORD wHigh = MAKEWORD(bByte2, bByte3);

	return MAKELONG(wLow, wHigh);
}

DWORD GetHalfByte(DWORD _dwData, int _nIndex)
{
	if(_nIndex <= 0 ||
		_nIndex > 8)
	{
		return 0;
	}

	WORD wData = 0;

	int nPtrOffset = _nIndex / 2;
	BYTE* pByte = (BYTE*)(&_dwData) + nPtrOffset;
	BYTE bData = *pByte;

	if(_nIndex % 2 == 0)
	{
		return GetLowHalfByte(bData);
	}
	else
	{
		return GetHighHalfByte(bData);
	}
}

DWORD SetHalfByte(DWORD _dwData, int _nIndex, BYTE _bData)
{
	if(_nIndex <= 0 ||
		_nIndex > 8)
	{
		return _dwData;
	}

	DWORD dwData = _dwData;
	WORD wData = 0;

	int nPtrOffset = _nIndex / 2;
	BYTE* pByte = (BYTE*)(&dwData) + nPtrOffset;
	BYTE bData = *pByte;

	if(_nIndex % 2 == 0)
	{
		BYTE bHigh = GetHighHalfByte(bData);
		*pByte = MakeByte(_bData, bHigh);
	}
	else
	{
		BYTE bLow = GetLowHalfByte(bData);
		*pByte = MakeByte(bLow, _bData);
	}

	return dwData;
}
//////////////////////////////////////////////////////////////////////////
RttTest::RttTest()
{
	m_pRenderTexture = NULL;
	m_pRenderSurface = NULL;
	m_pOldSurface = NULL;

	m_pTestDlg = new GameDisplayDlg;
	RECT rcWnd;
	rcWnd.left = rcWnd.top = 0;
	rcWnd.right = 300;
	rcWnd.bottom = 400;
	m_pTestDlg->SetRect(rcWnd);

	DWORD dwData = MakeDwordWithHalfBytes(0, 1, 2, 3, 4, 5, 6, 7);

	for(int i = 0; i <= 7; ++i)
	{
		BYTE bByte = GetHalfByte(dwData, i);
		int a = 0;
	}
}

RttTest::~RttTest()
{

}

bool RttTest::UserInitial()
{
	g_nScreenWidth = m_pHGE->System_GetState(HGE_SCREENWIDTH);
	g_nScreenHeight = m_pHGE->System_GetState(HGE_SCREENHEIGHT);

//	create rtt
	/*m_pRenderTexture->GetSurfaceLevel(0, &m_pRenderSurface);

	GetDevice()->GetRenderTarget(&m_pOldSurface);

	//	start render to texture
	if(m_pHGE->Gfx_BeginScene())
	{
		GetDevice()->SetRenderTarget(NULL, m_pRenderSurface);
		GetDevice()->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x0,
			1.0f,
			0);

		//	start to render
		HTEXTURE tex = m_pHGE->Texture_Load(GetFullPath("res/shaizi1.png"));
		hgeSprite spr(tex, 0, 0, 62, 72);
		spr.Render(0, 0);

		m_pHGE->

		//	release
		GetDevice()->EndScene();
		GetDevice()->SetRenderTarget(0, m_pOldSurface);
		m_pRenderSurface->Release();
		m_pRenderSurface = NULL;
		m_pOldSurface = NULL;
	}*/

	AfxSetHge(m_pHGE);

	//m_pTestDlg->AddStringItem("²âÊÔ", 50, 50);
	ItemAttrib item;
	ZeroMemory(&item, sizeof(ItemAttrib));
	strcpy(item.name, "Ð¡µ¶");
	item.type = 1;
	item.EXPR = 3;
	item.maxAC = 5;
	item.maxMAC = 411;
	item.HP = 392;
	item.MP =32;
	item.lucky = 1;
	item.hide = 4;
	m_pTestDlg->ShowItemAttrib(&item);

	return true;
}

bool RttTest::RealRenderFunc()
{
	//	before begin scene

	//	create rtt
	/*m_pRenderTexture->GetSurfaceLevel(0, &m_pRenderSurface);

	GetDevice()->GetRenderTarget(&m_pOldSurface);

	//	start render to texture
	if(m_pHGE->Gfx_BeginScene())
	{
		GetDevice()->SetRenderTarget(NULL, m_pRenderSurface);
		GetDevice()->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x0,
			1.0f,
			0);

		//	start to render
		HTEXTURE tex = m_pHGE->Texture_Load(GetFullPath("res/shaizi1.png"));
		hgeSprite spr(tex, 0, 0, 62, 72);
		spr.Render(0, 0);

		m_pHGE->

		//	release
		GetDevice()->EndScene();
		GetDevice()->SetRenderTarget(0, m_pOldSurface);
		m_pRenderSurface->Release();
		m_pRenderSurface = NULL;
		m_pOldSurface = NULL;
	}*/

	return __super::RealRenderFunc();
}

void RttTest::UserRenderFunc()
{
	//HTEXTURE tex = (HTEXTURE)m_pRenderTexture;
	//HTEXTURE tex = m_pHGE->Texture_Load(GetFullPath("res/shaizi1.png"));
	/*HTEXTURE tex = m_pHGE->Target_GetTexture(m_hTarget);
	hgeSprite spr(tex, 0, 0, 62, 72);
	spr.Render(50, 50);*/

	m_pTestDlg->Render();
}


void RttTest::UserFrameFunc()
{
	float x, y = 0;
	AfxGetHge()->Input_GetMousePos(&x, &y);
	POINT ptMouse = {x, y};
	m_pTestDlg->ProcUserCmd(ptMouse);
}