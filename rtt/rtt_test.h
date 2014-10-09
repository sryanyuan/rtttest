#include "SGameBase.h"
#include <d3d8.h>
#include <d3dx8.h>
#include "GameCommonDlg.h"
#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
class RttTest : public SGameBase
{
public:
	RttTest();
	virtual ~RttTest();

public:
	virtual bool RealRenderFunc();
	virtual bool UserInitial();
	virtual void UserRenderFunc();
	virtual void UserFrameFunc();  

protected:
	IDirect3DSurface8* m_pRenderSurface;
	IDirect3DTexture8* m_pRenderTexture;
	IDirect3DSurface8* m_pOldSurface;
	HTARGET m_hTarget;

	GameDisplayDlg* m_pTestDlg;
};