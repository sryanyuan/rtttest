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
//	装备属性
struct ItemAttrib
{
	//	0装备代号 唯一ID
	UINT id;	//	人物
	//	1装备名字
	char name[20];
	//	2幸运值
	//	对于药品 0:分成5份加 1:直接加
	//	对于卷轴 0:随机 2:回程	6:大陆传送卷
	//	对于无限可用用品 1:装备升级石头 2:乾坤石
	//	对于书籍 0:不限职业 1:战士 2:法师 3:道士
	BYTE lucky;
	//	3诅咒值
	//	对于怪物 0:非不死系 1:不死系 2:可召唤
	//	辨别ITEM_COST类别 0:药品 1:卷轴 3:无限可用物品 2:藏宝图 4:恶魔召唤包 5:金币
	BYTE curse;	
	//	4躲避
	//	对于武器 武器没有躲避值 武器的躲避为该武器砍下去的声音
	//	对于 消耗品 的 藏宝图 hide是副本的MapID
	//	对于 消耗品 的 药品 hide是冷却时间
	//	对于 消耗品 的 卷轴中的 大陆传送卷 hide是目标地图ID
	//	对于 消耗品 的 无限可用物品 中的 装备升级石头 1:攻击 2:魔法 3:道术
	//	对于 消耗品 的 金币来说 是能释放出多少百万金币
	BYTE hide;	//	
	//	5准确
	//	对于藏宝图 是副本挖宝地图的MapID
	//	对于 装备升级石头 是该装备升级石头对应的等级
	BYTE accuracy;
	//	6攻击速度
	//	对于消耗品 是消耗品数量 < 20
	BYTE atkSpeed;
	//	7麻痹几率
	//	对于装备 是套装属性ID
	BYTE atkPalsy;
	//	8中毒几率
	//	决定是否绑定 已不用
	BYTE atkPois;
	//	9移动速度
	BYTE moveSpeed;
	//	10重量
	//	对于怪物 有WEIGHT_MASK决定怪物属性
	BYTE weight;	
	//	11要求类型(N)
	//	1 = 等级 2 = 攻击 3 = 魔法力 4 = 道术 5 = 性别
	BYTE reqType;
	//	12要求值(N)
	//	0 = 通用 0 = 战士 1 = 法师 2 = 道士   ||0 = 男 1 = 女
	BYTE reqValue;
	//	13男女(N)
	BYTE sex;
	//	14种类(N)
	BYTE type;
	//	15攻击力
	//	对于药品 为解捆后的物品ID
	//	对于藏宝图 为藏宝地图的x坐标 对于怪物召唤包 为怪物ID
	USHORT maxDC;	
	//	对于藏宝图 为藏宝地图的y坐标
	USHORT DC;
	//	17防御力
	//	怪物只有固定AC MAC maxAC LoWord: 当前等级 HiWord:初始等级
	//	对于藏宝图 为挖宝地图的x坐标
	USHORT maxAC;	
	//	对于藏宝图 为挖宝地图的y坐标
	USHORT AC;
	//	19魔防
	//	怪物获取的经验值
	USHORT maxMAC;	
	USHORT MAC;
	//	21道术
	//	用于确定怪物的攻击距离
	USHORT maxSC;
	//	用于确定怪物视野
	USHORT SC;
	//	23魔法力
	//	用于确定怪物的攻击间隔
	USHORT maxMC;	
	//	用于确定怪物的行走间隔
	USHORT MC;		
	//	25血量
	//	对于装备 装备持久 高位 当前持久  低位 最大持久
	UINT maxHP;	
	//	装备的特效外观 人物周围的光
	UINT HP;
	//	27魔
	UINT maxMP;		
	UINT MP;	
	//	29经验
	//	最大经验MASK(对于怪物)
	UINT maxEXPR;
	//	经验的MASK(对于ITEM)
	UINT EXPR;
	//	31装备的要求等级 在reqType上面
	USHORT level;	//	低位为装备增加的点数
	//	32附加
	USHORT extra;
	//	33图片索引
	//	对于怪物 tex为召唤怪物ID
	USHORT tex;

	//	34价格
	USHORT price;
	//	35服务器标签 任何物品唯一
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
	//	是否可见
	bool m_bVisible;
	//	物体矩形
	RECT m_rcClient;
	//	当前帧
	BYTE m_bCurFrame;
	//RECT m_rcObject;
	//	是否获取焦点
	bool m_bFocus;
	//	绘图精灵
	hgeSprite* m_pRender;
	//	是否静态(对于对话框来说 静态可以阻止用户输入)
	bool m_bIsStatic;
	//	是否需要提到顶层
	bool m_bNeedTopLevel;
private:
	//	物体ID
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