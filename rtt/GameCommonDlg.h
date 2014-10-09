#ifndef GAMECOMMONDLG_H_
#define GAMECOMMONDLG_H_

//	ÓÎÏ·¶Ô»°¿ò

#include "GameRenderObject.h"
#include <vector>
//////////////////////////////////////////////////////////////////////////
struct CommonButton
{
	int nID;
	int nType;
	BYTE bState;
	RECT stPos;
	char szText[9];
};
typedef std::vector<CommonButton> COMMONBUTTONS;

#define CBTN_NORMAL		1
#define CBTN_LEFTARROW	2
#define CBTN_RIGHTARROW	3
#define CBTN_UPARROW	4
#define CBTN_DOWNARROW	5
#define CBTN_CHECKBOX	6

#define BTN_NORMAL_MASK		0x00
#define BTN_OVER_MASK		0x80
#define BTN_DOWN_MASK		0x40

#define BTN_STATE_NORMAL	0
#define BTN_STATE_OVER		1
#define BTN_STATE_DOWN		2

#define BTN_STATE_UNCHECKED	3
#define BTN_STATE_CHECKED	4
//////////////////////////////////////////////////////////////////////////
class GameCommonDlg : public RenderObject
{
public:
	GameCommonDlg();
	~GameCommonDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual void ResetStatus();

public:
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void OnMoveStop(){}
	virtual bool OnCommonButtonClick(int _id){return false;}
	virtual void AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText);

public:
	void RemoveCommonButton(int _id);
	void MoveTo(int _x, int _y);
	inline int GetWidth()			{return m_rcClient.right - m_rcClient.left;}
	inline int GetHeight()			{return m_rcClient.bottom - m_rcClient.top;}

protected:
	bool m_bLButtonDown;
	POINT m_ptOrigin;

protected:
	RECT m_rcCloseBtn;
	BYTE m_bCloseState;
	COMMONBUTTONS m_xButtons;
};

class GameNewDlg : public GameCommonDlg
{
public:
	GameNewDlg();
	virtual ~GameNewDlg();

public:
	virtual void Render();
	virtual void Update(float _dt){}
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText);
	virtual bool OnCloseDialog()			{return true;}
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void CloseDialog();
	virtual void ShowDialog();
	virtual bool OnShowDialog()				{return true;}

public:
	bool CalcCloseButtonPosition();
	inline void SetWindowTitle(const char* _pszTitle)
	{
		m_xTitle = _pszTitle;
		m_nTitleWidth = GetTextWidth(m_xTitle.c_str(), 12);
	}
	inline void SetWindowInfo(const char* _pszInfo)
	{
		m_xInfo = _pszInfo;
		m_nInfoWidth = GetTextWidth(m_xInfo.c_str(), 12);
	}
	inline void SetFixed()
	{
		m_bCanMove = false;
	}
	inline void SetCustomBk()
	{
		m_bCustomBk = true;
	}
	inline void SetAlpha(DWORD _dwAlpha)
	{
		m_dwAlpha = _dwAlpha;
		UpdateAlpha();
	}

	CommonButton* GetCommonButtonData(int _id);

protected:
	void UpdateAlpha();

protected:
	std::string m_xTitle;
	std::string m_xInfo;
	int m_nTitleWidth;
	int m_nInfoWidth;
	bool m_bCanMove;
	bool m_bCustomBk;
	DWORD m_dwAlpha;
};

#endif