#include "SGameBase.h"

#include <Windows.h>  
#include <assert.h>  
#include <stdio.h>  
#include <io.h>  

//////////////////////////////////////////////////////  
//  
//  构造析构  
//  
///////////////////////////////////////////////////////  

SGameBase::SGameBase(const char* szTitle)  
{  
	m_pGUI=NULL;  
	m_pHGE=NULL;  
	m_pSprCursor=NULL;  


	//得到自身实例  
	SGameBase::InitialInstance(this);  

	if(!szTitle)  
	{  
		memset(m_szGameTitle,0,sizeof(m_szGameTitle));  
	}  
	else  
	{  
		strcpy(m_szGameTitle,szTitle);  
	}  

	this->HgeInitial(m_szGameTitle);  

	SGameBase::InitialProc((WNDPROC)GetWindowLong(m_pHGE->System_GetState(HGE_HWND),GWL_WNDPROC));  
	SetWindowLong(m_pHGE->System_GetState(HGE_HWND),GWL_WNDPROC,(LONG)SGameBase::MainWinProc);  

	m_pDev = NULL;
}  

SGameBase::~SGameBase()  
{  
	this->HgeUninitial();  
}  

//////////////////////////////////////////////////////  
//  
//  静态函数实现  
//  
///////////////////////////////////////////////////////  

SGameBase*            SGameBase::InitialInstance(SGameBase * p/* =NULL */)  
{  
	//保存一个实例  
	//参数空则返回上次初始化的实例  

	static SGameBase* pInstance=NULL;  

	if(!p)  
	{  
		return pInstance;  
	}  

	pInstance=p;  
	return pInstance;  
}  

WNDPROC             SGameBase::InitialProc(WNDPROC p)  
{  
	static WNDPROC pFunc=NULL;  

	if(!p)  
	{  
		return pFunc;  
	}  

	pFunc=p;  
	return pFunc;  
}  

LRESULT             SGameBase::MainWinProc(HWND h, UINT u, WPARAM w, LPARAM l)  
{  
	SGameBase::InitialInstance()->WinEventFilter(h,u,w,l);  
	return CallWindowProc(SGameBase::InitialProc(),h,u,w,l);
}  

bool                SGameBase::FrameFunc()  
{  
	return SGameBase::InitialInstance()->RealFrameFunc();  
}  

bool                SGameBase::RenderFunc()  
{  
	return SGameBase::InitialInstance()->RealRenderFunc();  
}  

bool                SGameBase::FocusGainFunc()  
{  
	return SGameBase::InitialInstance()->RealFocusGainFunc();  
}  

bool                SGameBase::FocusLostFunc()  
{  
	return SGameBase::InitialInstance()->RealFocusLostFunc();  
}  

bool                SGameBase::GfxRestoreFrameFunc()  
{  
	return SGameBase::InitialInstance()->RealGfxRestoreFunc();  
}  



//////////////////////////////////////////////////////  
//  
//  初始化及清理函数  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::HgeInitial(const char* szTitle)  
{  
	m_pHGE=hgeCreate(HGE_VERSION);  
	m_pGUI=new hgeGUI;  

	assert(m_pHGE);  
	assert(m_pGUI);  
	//settings  

	m_pHGE->System_SetState(HGE_FRAMEFUNC,       SGameBase::FrameFunc);  
	m_pHGE->System_SetState(HGE_RENDERFUNC,      SGameBase::RenderFunc);  
	m_pHGE->System_SetState(HGE_FOCUSGAINFUNC,   SGameBase::FocusGainFunc);  
	m_pHGE->System_SetState(HGE_FOCUSLOSTFUNC,   SGameBase::FocusLostFunc);  
	m_pHGE->System_SetState(HGE_GFXRESTOREFUNC,  SGameBase::GfxRestoreFrameFunc);  



	m_pHGE->System_SetState(HGE_DONTSUSPEND,true);  
	m_pHGE->System_SetState(HGE_SHOWSPLASH,  false);  
	m_pHGE->System_SetState(HGE_FPS,     30);  
	m_pHGE->System_SetState(HGE_SCREENBPP,   32);  
	m_pHGE->System_SetState(HGE_TITLE,       szTitle);  
	m_pHGE->System_SetState(HGE_USESOUND,    false);  
	m_pHGE->System_SetState(HGE_WINDOWED,    true);  
	m_pHGE->System_SetState(HGE_HIDEMOUSE,   false);  

	m_bHgeInitial=m_pHGE->System_Initiate();  
	return true;  
}  

void                SGameBase::HgeUninitial()  
{  
	this->UserUninitial();  
	m_pHGE->System_Shutdown();  
	m_pHGE->Release();  
}  

bool                SGameBase::UserInitial()  
{  
	return true;  
}  

void                SGameBase::UserUninitial()  
{  

}  
//////////////////////////////////////////////////////  
//  
//  辅助函数  
//  
///////////////////////////////////////////////////////  

bool				SGameBase::IsCaptionBar(int x,int y)
{
	if(y <= GetSystemMetrics(SM_CYCAPTION))  
	{  
		return true;  
	}  

	return false;  
}
//////////////////////////////////////////////////////  
//  
//  过滤消息的窗口过程  
//  
///////////////////////////////////////////////////////  
LRESULT             SGameBase::WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l)  
{  
	return w;  
}  
//////////////////////////////////////////////////////  
//  
//  逻辑和渲染函数实现  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::RealFocusGainFunc()  
{  
	return false;  
}  

bool                SGameBase::RealFocusLostFunc()  
{  
	return false;  
}  

bool            SGameBase::RealGfxRestoreFunc()  
{  
	return true;  
}  

bool                SGameBase::RealFrameFunc()  
{  
	this->UpdateCommand();  
	this->UserFrameFunc();  
	return this->ProcCommand();  
}  

bool                SGameBase::RealRenderFunc()  
{  
	m_pHGE->Gfx_BeginScene();  
	m_pHGE->Gfx_Clear(0xffffffff);  
	this->UserRenderFunc();  
	m_pGUI->Render();  
	m_pHGE->Gfx_EndScene();  
	return false;  
}  

void                SGameBase::UpdateCommand()  
{  
	m_fTimeDelta=m_pHGE->Timer_GetDelta();  
	m_nCommandID=m_pGUI->Update(m_fTimeDelta);  
}  

bool                SGameBase::ProcCommand()  
{  
	if(m_pHGE->Input_GetKeyState(HGEK_ESCAPE))  
	{  
		return true;  
	}  

	//处理GUI消息  
	switch(this->m_nCommandID)  
	{  
	default:  
		break;  
	}  

	//若退出 在switch语句中返回true  
	return false;  
}  

void                SGameBase::UserFrameFunc()  
{  
	//用户实现  
}  

void                SGameBase::UserRenderFunc()  
{  
	//用户实现  
}  
//////////////////////////////////////////////////////  
//  
//  外部接口  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::SetGameCursor(const char* szCursorPath)  
{  
	m_hCursor=m_pHGE->Texture_Load(szCursorPath);  
	if(m_hCursor)  
	{  
		m_pSprCursor=new hgeSprite(m_hCursor,
			0.0f,0.0f,
			m_pHGE->Texture_GetWidth(m_hCursor,true),
			m_pHGE->Texture_GetHeight(m_hCursor,true));  
		m_pSprCursor->SetZ(1);  
		m_pSprCursor->SetHotSpot(0,0);  
		m_pSprCursor->SetColor(0xFFFFFFFF);  
		m_pGUI->SetCursor(m_pSprCursor);  
		return true;  
	}  
	S_DEBUG;  
	return false;  
}  

void                SGameBase::EnableSound(bool b)  
{  
	m_pHGE->System_SetState(HGE_USESOUND,b);  
}  

bool                SGameBase::SetGameTitle(const char* szTitle)  
{  
	if(strlen(szTitle) > sizeof(m_szGameTitle))  
	{  
		return false;  
	}  
	strcpy(m_szGameTitle,szTitle);  
	m_pHGE->System_SetState(HGE_TITLE,m_szGameTitle);  
	return true;  
}  

void				SGameBase::SetGameFps(int nFps)
{
	m_pHGE->System_SetState(HGE_FPS,nFps);
}

void                SGameBase::Exec()  
{  
	if(m_bHgeInitial)  
	{  
		//	get direct3d device
		HTEXTURE tex = m_pHGE->Texture_Create(1, 1);
		LPDIRECT3DTEXTURE8 pTex = (LPDIRECT3DTEXTURE8)tex;
		HRESULT hr = pTex->GetDevice((IDirect3DDevice8**)&m_pDev);
		m_pHGE->Texture_Free(tex);

		if(FAILED(hr))
		{
			MessageBox(NULL, "Can't get the device pointer.", "error", MB_ICONERROR | MB_OK);
			abort();
		}

		if(this->UserInitial())  
		{  
			m_pHGE->System_Start();  
		}  
		else  
		{  
			abort();  
		}  
	}  
	else  
	{  
		abort();  
	}  
}  

void				SGameBase::SetGameLogFile(const char* szFile)
{
	m_pHGE->System_SetState(HGE_LOGFILE,szFile);
}

void				SGameBase::AddLog(const char* szLog)
{
	m_pHGE->System_Log(szLog);
}