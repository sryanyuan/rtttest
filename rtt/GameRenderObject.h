#ifndef GAMERENDEROBJECT_H_
#define GAMERENDEROBJECT_H_

#include <hge.h>
#include <hgesprite.h>
#include <list>
#include <vector>
#include <algorithm>
#include "TextPrinter.h"
#include <hgeresource.h>
//////////////////////////////////////////////////////////////////////////
#define INVALID_TEXTURE_INDEX	-1

#define COLOR_EFF_STONE			ARGB(255, 100, 100, 100)
#define COLOR_EFF_ICE			ARGB(96, 75, 255, 255)

#define OBJECT_CHAT_EACHLINE	(12 * 12)

#define SAFE_DELETE(p)			do{if(p){delete p;p=NULL;}}while(0);

#define RELATIVE_X(x)			(m_rcClient.left + x)
#define RELATIVE_Y(y)			(m_rcClient.top + y)

#define ARGB_WHITE				ARGB(255, 255, 255, 255)
#define ARGB_GREEN				ARGB(255, 0, 255, 0)
#define ARGB_RED				ARGB(255, 255, 0, 0)
#define ARGB_YELLOW				ARGB(255, 255, 255, 0)

#define TEST_FLAG_BOOL(flag,mask)	((flag & mask ) != 0)
//////////////////////////////////////////////////////////////////////////
unsigned int GetTextWidth(const char* _pText, size_t _nFontSize);
TextPrinter* AfxGetPrinter();
GfxFont* AfxGetFont();
GfxFont* AfxGetFont14();
HGE* AfxGetHge();
void AfxSetHge(HGE* _pHge);

extern hgeResourceManager* g_pResMgr;
//////////////////////////////////////////////////////////////////////////
//	װ������
struct ItemAttrib
{
	//	0װ������ ΨһID
	UINT id;	//	����
	//	1װ������
	char name[20];
	//	2����ֵ
	//	����ҩƷ 0:�ֳ�5�ݼ� 1:ֱ�Ӽ�
	//	���ھ��� 0:��� 2:�س�	6:��½���;�
	//	�������޿�����Ʒ 1:װ������ʯͷ 2:Ǭ��ʯ
	//	�����鼮 0:����ְҵ 1:սʿ 2:��ʦ 3:��ʿ
	BYTE lucky;
	//	3����ֵ
	//	���ڹ��� 0:�ǲ���ϵ 1:����ϵ 2:���ٻ�
	//	���ITEM_COST��� 0:ҩƷ 1:���� 3:���޿�����Ʒ 2:�ر�ͼ 4:��ħ�ٻ��� 5:���
	BYTE curse;	
	//	4���
	//	�������� ����û�ж��ֵ �����Ķ��Ϊ����������ȥ������
	//	���� ����Ʒ �� �ر�ͼ hide�Ǹ�����MapID
	//	���� ����Ʒ �� ҩƷ hide����ȴʱ��
	//	���� ����Ʒ �� �����е� ��½���;� hide��Ŀ���ͼID
	//	���� ����Ʒ �� ���޿�����Ʒ �е� װ������ʯͷ 1:���� 2:ħ�� 3:����
	//	���� ����Ʒ �� �����˵ �����ͷų����ٰ�����
	BYTE hide;	//	
	//	5׼ȷ
	//	���ڲر�ͼ �Ǹ����ڱ���ͼ��MapID
	//	���� װ������ʯͷ �Ǹ�װ������ʯͷ��Ӧ�ĵȼ�
	BYTE accuracy;
	//	6�����ٶ�
	//	��������Ʒ ������Ʒ���� < 20
	BYTE atkSpeed;
	//	7��Լ���
	//	����װ�� ����װ����ID
	BYTE atkPalsy;
	//	8�ж�����
	//	�����Ƿ�� �Ѳ���
	BYTE atkPois;
	//	9�ƶ��ٶ�
	BYTE moveSpeed;
	//	10����
	//	���ڹ��� ��WEIGHT_MASK������������
	BYTE weight;	
	//	11Ҫ������(N)
	//	1 = �ȼ� 2 = ���� 3 = ħ���� 4 = ���� 5 = �Ա�
	BYTE reqType;
	//	12Ҫ��ֵ(N)
	//	0 = ͨ�� 0 = սʿ 1 = ��ʦ 2 = ��ʿ   ||0 = �� 1 = Ů
	BYTE reqValue;
	//	13��Ů(N)
	BYTE sex;
	//	14����(N)
	BYTE type;
	//	15������
	//	����ҩƷ Ϊ���������ƷID
	//	���ڲر�ͼ Ϊ�ر���ͼ��x���� ���ڹ����ٻ��� Ϊ����ID
	USHORT maxDC;	
	//	���ڲر�ͼ Ϊ�ر���ͼ��y����
	USHORT DC;
	//	17������
	//	����ֻ�й̶�AC MAC maxAC LoWord: ��ǰ�ȼ� HiWord:��ʼ�ȼ�
	//	���ڲر�ͼ Ϊ�ڱ���ͼ��x����
	USHORT maxAC;	
	//	���ڲر�ͼ Ϊ�ڱ���ͼ��y����
	USHORT AC;
	//	19ħ��
	//	�����ȡ�ľ���ֵ
	USHORT maxMAC;	
	USHORT MAC;
	//	21����
	//	����ȷ������Ĺ�������
	USHORT maxSC;
	//	����ȷ��������Ұ
	USHORT SC;
	//	23ħ����
	//	����ȷ������Ĺ������
	USHORT maxMC;	
	//	����ȷ����������߼��
	USHORT MC;		
	//	25Ѫ��
	//	����װ�� װ���־� ��λ ��ǰ�־�  ��λ ���־�
	UINT maxHP;	
	//	װ������Ч��� ������Χ�Ĺ�
	UINT HP;
	//	27ħ
	UINT maxMP;		
	UINT MP;	
	//	29����
	//	�����MASK(���ڹ���)
	UINT maxEXPR;
	//	�����MASK(����ITEM)
	UINT EXPR;
	//	31װ����Ҫ��ȼ� ��reqType����
	USHORT level;	//	��λΪװ�����ӵĵ���
	//	32����
	USHORT extra;
	//	33ͼƬ����
	//	���ڹ��� texΪ�ٻ�����ID
	USHORT tex;

	//	34�۸�
	USHORT price;
	//	35��������ǩ �κ���ƷΨһ
	DWORD tag;
};
//////////////////////////////////////////////////////////////////////////

class RenderObject;

using std::list;
using std::vector;
typedef list<RenderObject*> OBJECTLIST;

class RenderObject
{
public:
	RenderObject()
	{
		static UINT id = 0;
		m_id = id++;
		m_bCurFrame = 0;
		m_bVisible = true;
		m_bFocus = false;
		ZeroMemory(&m_rcClient, sizeof(m_rcClient));
		m_pRender = NULL;
		m_bIsStatic = false;
		m_bNeedTopLevel = false;
	}
	virtual ~RenderObject()
	{
		SAFE_DELETE(m_pRender);
	}

public:
	virtual void Render() = 0;
	virtual void Update(float _dt) = 0;
	virtual bool IsLastFrame(){return true;}
	virtual bool ProcUserCmd(const POINT& _mp)
	{
		return true;
	}
public:
	virtual bool LoadProperty(int _id){return true;}
	virtual int OnLButtonDown(){return 0;}
	virtual int OnRButtonDown(){return 0;}
	virtual void ResetTimerCounter(){}
	virtual void ResetStatus(){}

public:
	inline bool IsStatic()
	{
		return m_bIsStatic;
	}
	inline void SetStatic(bool _s)
	{
		m_bIsStatic = _s;
	}
	inline void SetVisible(bool _vb)
	{
		if(!_vb)
		{
			//m_bFocus = false;
		}
		m_bVisible = _vb;
	}
	inline bool IsVisible() const
	{
		return m_bVisible;
	}
	inline void SetRect(const RECT& rc)
	{
		m_rcClient = rc;
	}
	inline const RECT& GetRect()
	{
		return m_rcClient;
	}
	inline BYTE GetCurFrame() const
	{
		return m_bCurFrame;
	}
	inline void SetCurFrame(BYTE _frm)
	{
		m_bCurFrame = _frm;
	}
	inline UINT GetID() const
	{
		return m_id;
	}
	inline bool IsPtInObject(const POINT& _pt)
	{
		bool bInObject = false;
		bInObject = ::PtInRect(&m_rcClient, _pt) ? true : false;
		return bInObject;
	}
	inline bool IsNeedTopLevel()
	{
		return m_bNeedTopLevel;
	}
	inline void SetNeedTopLevel(bool _b)
	{
		m_bNeedTopLevel = _b;
	}

protected:
	//	�Ƿ�ɼ�
	bool m_bVisible;
	//	�������
	RECT m_rcClient;
	//	��ǰ֡
	BYTE m_bCurFrame;
	//RECT m_rcObject;
	//	�Ƿ��ȡ����
	bool m_bFocus;
	//	��ͼ����
	hgeSprite* m_pRender;
	//	�Ƿ�̬(���ڶԻ�����˵ ��̬������ֹ�û�����)
	bool m_bIsStatic;
	//	�Ƿ���Ҫ�ᵽ����
	bool m_bNeedTopLevel;
private:
	//	����ID
	UINT m_id;
};


class RenderTarget : public RenderObject
{
public:
	RenderTarget(){}
	~RenderTarget(){}

public:
	virtual void Update(float _dt)
	{
		for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			(*iter)->Update(_dt);
		}
	}
	virtual void Render()
	{
		for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			if((*iter)->IsVisible())
			{
				(*iter)->Render();
			}
		}
	}

public:
	virtual void InsertObject(RenderObject* _obj)
	{
		m_objList.push_back(_obj);
	}
	virtual RenderObject* RemoveObject(int _id)
	{
		RenderObject* tobj = NULL;
		for(OBJECTLIST::iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			if((*iter)->GetID() == _id)
			{
				tobj = (*iter);
				m_objList.erase(iter);
				break;
			}
		}
		return tobj;
	}
	void BringToTop(RenderObject* _obj)
	{
		OBJECTLIST::iterator iter = std::find(m_objList.begin(), m_objList.end(), _obj);
		if(iter != m_objList.end())
		{
			m_objList.erase(iter);
		}
		m_objList.push_back(_obj);
	}

protected:
	OBJECTLIST m_objList;
};

class RenderDlgControl : public RenderTarget
{
public:
	RenderDlgControl()
	{
		m_pStaticDlg = NULL;
	}
public:
	inline bool IsBlockCmd()
	{
		return m_pStaticDlg != NULL;
	}
	inline void SetUnBlock()
	{
		m_pStaticDlg = NULL;
	}
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render()
	{
		{
			for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
			{
				if((*iter)->IsVisible())
				{
					(*iter)->Render();
				}
			}
		}
	}
	virtual RenderObject* RemoveObject(int _id)
	{
		if(m_pStaticDlg)
		{
			if(m_pStaticDlg->GetID() == _id)
			{
				m_pStaticDlg = NULL;
			}
		}
		return __super::RemoveObject(_id);
	}

protected:
	RenderObject* m_pStaticDlg;
	//OBJECTLIST m_xNeedTopLevel;
};

//////////////////////////////////////////////////////////////////////////
enum OBJECT_RENDER_MODE
{
	ORM_NONE,
	ORM_NORMAL,
	ORM_TRANSPARENT,
	ORM_HIGHLIGHT
};

#endif