#ifndef _SGAMEBASE_H
#define _SGAMEBASE_H

#include <hge.h>  
#include <hgegui.h>  
#include <hgeSprite.h>   
#include <hgefont.h>  

#pragma comment(lib,"HGE.lib")
#pragma comment(lib,"HGEHelp.lib")

#include <d3d8.h>
#include <d3dx8.h>

//////////////////////////////////////////////////////  
//  
//  定义  
//  
///////////////////////////////////////////////////////  




//宏  
#define ENABLE_DEBUG  

#ifdef ENABLE_DEBUG  
#define S_DEBUG\
	{MessageBoxA(NULL,\
		m_pHGE->System_GetErrorMessage(),\
		"HGE Error Code",\
		MB_OK|MB_ICONERROR);}  
#define S_DEBUG_(M)\
	{char buf[200];\
		sprintf(buf,"Line:%d    Function:%s   Run;",__LINE__,__FUNCTION__);\
		MessageBoxA(NULL,\
		buf,\
		M,\
		MB_OK);}  
#else  
#define S_DEBUG    
#define S_DEBUG_(M)    
#endif  

////////////////////////////////  


class SGameBase  
{  
public:  
	SGameBase(const char* szTitle=NULL);  
	virtual ~SGameBase();  

public:  
	//静态函数用来调用实例的成员函数  
	static bool                     FrameFunc();  
	static bool                     RenderFunc();  
	static bool                     FocusGainFunc();  
	static bool                     FocusLostFunc();  
	static bool                     GfxRestoreFrameFunc();  
	//窗口过程  
	static LRESULT                  MainWinProc(HWND h, UINT u, WPARAM w, LPARAM l);  


public:  
	//外部接口
	bool						    SetGameCursor(const char* szCursorPath);  
	bool						    SetGameTitle(const char* szTitle);  
	void							SetGameFps(int nFps);
	void							SetGameLogFile(const char* szFile);
	void							AddLog(const char* szLog);

	char*                           GetGameTitle(){return m_szGameTitle;}  
	const char*                     GetGameTitle()const{return m_szGameTitle;}  

	void				            EnableSound(bool);  
	void                            Exec();  

	inline	IDirect3DDevice8*		GetDevice()		{return m_pDev;}

protected:  
	//获取实例指针  
	static SGameBase *              InitialInstance(SGameBase * p=NULL);  
	//获取实例窗口过程  
	static WNDPROC                  InitialProc(WNDPROC p=NULL);  

private:  
	bool                            HgeInitial(const char* szTitle);  
	void                            HgeUninitial();  

protected:  
	//事件过滤器
	virtual LRESULT                 WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l);  

	//真实的处理函数
	virtual bool                    RealFrameFunc();  
	virtual bool                    RealRenderFunc();  
	virtual bool                    RealFocusGainFunc();  
	virtual bool                    RealFocusLostFunc();  
	virtual bool                    RealGfxRestoreFunc();  

	//用户处理函数  
	virtual bool                    UserInitial();  
	virtual void                    UserUninitial();  
	virtual void                    UserFrameFunc();  
	virtual void                    UserRenderFunc();  

	//GUI操作处理函数  
	virtual void                    UpdateCommand();  
	virtual bool                    ProcCommand();  

	//辅助函数
	virtual bool					IsCaptionBar(int x,int y);

protected:  
	HGE *                           m_pHGE;  
	hgeGUI *                        m_pGUI;  
	int                             m_nCommandID;  

private:  
	HTEXTURE                        m_hCursor;  
	char                            m_szGameTitle[50];  
	hgeSprite*                      m_pSprCursor;  
	bool                            m_bHgeInitial;  
	float                           m_fTimeDelta;  

	IDirect3DDevice8*				m_pDev;
};  

#endif