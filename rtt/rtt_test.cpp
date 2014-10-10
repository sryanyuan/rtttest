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
}

RttTest::~RttTest()
{

}

bool RttTest::UserInitial()
{
	g_nScreenWidth = m_pHGE->System_GetState(HGE_SCREENWIDTH);
	g_nScreenHeight = m_pHGE->System_GetState(HGE_SCREENHEIGHT);

	/*m_hTarget = m_pHGE->Target_Create(g_nScreenWidth, g_nScreenHeight, true);
	m_pHGE->Gfx_BeginScene(m_hTarget);
	m_pHGE->Gfx_Clear(0);
	HTEXTURE tex = m_pHGE->Texture_Load(GetFullPath("res/shaizi1.png"));
	hgeSprite spr(tex, 0, 0, 62, 72);
	spr.Render(0, 0);
	m_pHGE->Gfx_EndScene();
*/

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