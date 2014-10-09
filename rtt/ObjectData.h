#ifndef OBJECTDATA_H_
#define OBJECTDATA_H_
#include <Windows.h>
#include <list>
#include <vector>

struct ItemAttrib;

typedef std::list<ItemAttrib*> PitemList;

#define REQUEST_LEVEL	 0x80
#define REQUEST_DC		 0x40
#define REQUEST_MC		 0x20
#define REQUEST_SC		 0x10

//	杂物
#define ITEM_NO			0
//	书
#define ITEM_BOOK		1
//	衣服
#define ITEM_CLOTH		2
//	项链
#define ITEM_NECKLACE	3
//	手镯
#define ITEM_BRACELAT	4
//	戒指
#define ITEM_RING		5
//	勋章
#define ITEM_MEDAL		6
//	头盔
#define ITEM_HELMET		7
//	药品
#define ITEM_YAO_SPE	8
//	武器
#define ITEM_WEAPON		9
//	杂物
#define ITEM_OTHER		10
//	普通药品 可以使用的物品(药品 卷轴)
#define ITEM_COST		11
//	捆
#define ITEM_BALE		12
//	卷轴
#define ITEM_SCROLL		13
//	鞋子
#define ITEM_SHOE		14
//	腰带
#define ITEM_BELT		15
//	宝石
#define ITEM_GEM		16
//	符咒
#define ITEM_CHARM		17
//	正在查询
#define ITEM_INQUERY	255

#define REQ_LEVEL		1
#define REQ_DC			2
#define REQ_MC			3
#define REQ_SC			4
#define REQ_SEX			5

#define REQ_ERR_LEVEL	1
#define REQ_ERR_DC		2
#define REQ_ERR_MC		3
#define REQ_ERR_SC		4
#define REQ_ERR_SEX		5
#define REQ_ERR_NOTHING	100
#define REQ_ERR_ISTEP	101
#define REQ_ERR_UNKNOWREQ	102

#define MAX_ATTACK_INTERVAL 140.0f
#define MAX_MOVE_INTERVAL   160.0f

#define NORMAL_WALK_COST_TIME	600

//	背包容量
#define MAIN_BAG_SIZE	36
#define ASSIST_BAG_SIZE	6
//	怪物属性 Weight值
#define WEIGHT_MASK_MGCATK	0x01
#define WEIGHT_MASK_SEEHIDE	0x02
#define WEIGHT_MASK_UNMOVE	0x04
#define WEIGHT_MASK_DEFFIRE	0x08
#define WEIGHT_MASK_STONE	0x10
#define WEIGHT_MASK_POISON	0x20
#define WEIGHT_MASK_IGNOREAC	0x40
#define WEIGHT_MASK_DEFICE	0x80
//	装备属性 Pois值
#define POIS_MASK_BIND		0x01
//	怪物属性

//	装备属性 Expr值
#define EXPR_MASK_NOSAVE	0x00000001
#define EXPR_MASK_NOSELL	0x00000002
//	怪物属性 MaxExpr值
#define MAXEXPR_MASK_BOSS		0x00000001
#define MAXEXPR_MASK_DEADHIDE	0x00000002
#define MAXEXPR_MASK_ELITE		0x00000004
#define MAXEXPR_MASK_LEADER		0x00000008

//	隐藏玩家 只针对玩家
#define TEX_MASK_HIDE		0x00000001

#define EXTRA_MASK_ENCRYPT	0x0001

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

struct GroundItem
{
	ItemAttrib stAttrib;
	WORD wPosX;
	WORD wPosY;
	//	对应服务器上生成的物品ID
	DWORD wID;
};

typedef std::vector<ItemAttrib> ItemList;
typedef std::list<GroundItem*>	GROUNDITEMS;

//	绘制信息
struct RenderInfo
{
	BYTE wil;
	BYTE id;
	BYTE drts;
	BYTE index;
	USHORT	standbegin;
	USHORT	standframe;
	USHORT	standtotal;
	USHORT	walkbegin;
	USHORT	walkframe;
	USHORT	walktotal;
	USHORT	atkbegin;
	USHORT	atkframe;
	USHORT	atktotal;
	USHORT	hurtbegin;
	USHORT	hurtframe;
	USHORT	hurttotal;
	USHORT	deadbegin;
	USHORT	deadframe;
	USHORT	deadtotal;
	USHORT	magicbegin;
	USHORT	magicframe;
	USHORT	magictotal;
	USHORT	atkeftbegin;
	USHORT	atkeftframe;
	USHORT	atkefttotal;

	USHORT	showbegin;
	USHORT	showframe;

	USHORT	sndindex;
};

//	玩家装备信息

struct PlayerEquipAttrib
{
	//	玩家武器
	int nWeapon;
	//	玩家衣服
	int nCloth;
	//	玩家项链
	int nNeck;
	//	玩家手镯
	int nBrac[2];
	//	玩家戒指
	int nRing[2];
	//	玩家头盔
	int nHelm;
	//	玩家勋章
	int nMedal;
	//	玩家宝石
	int nGem;
	//	玩家鞋子
	int nShoe;
	//	玩家腰带
	int nBelt;
	//	玩家发型
	int nHairStyle;
};

//	装备索引信息
struct PlayerTexIndexTable
{
	//	人物索引
	USHORT wHum;
	//	发型
	USHORT wHair;
	//	武器
	USHORT wWeapon;
};

//	地图怪物装备信息
class GameObject;
struct GroundItem;
class MagicElement;
#define MAX_CELL_OBJECTS	5
struct MemMapObject
{
	//	物体
	GameObject* pObjects[MAX_CELL_OBJECTS];
	//	装备
	GroundItem* pItems[MAX_CELL_OBJECTS];
	//	魔法
	MagicElement* pMagics[MAX_CELL_OBJECTS];
};

#define PLAYER_ITEM_TOTAL116	9

enum PLAYER_ITEM_TYPE
{
	PLAYER_ITEM_WEAPON,
	PLAYER_ITEM_CLOTH,
	PLAYER_ITEM_HELMET,
	PLAYER_ITEM_NECKLACE,
	PLAYER_ITEM_BRACELAT1,
	PLAYER_ITEM_BRACELAT2,
	PLAYER_ITEM_RING1,
	PLAYER_ITEM_RING2,
	PLAYER_ITEM_MEDAL,
	PLAYER_ITEM_BELT,
	PLAYER_ITEM_SHOE,
	PLAYER_ITEM_GEM,
	PLAYER_ITEM_CHARM,
	PLAYER_ITEM_TOTAL,
};

//	魔法效果
enum MAGICEFFECT_TYPE
{
	ME_NONE = 0,
	ME_GONGSHA = 7,
	ME_CISHA = 16,
	ME_BANYUE = 23,
	ME_LIEHUO = 43,
	ME_HONGDU,
	ME_LVDU,
	ME_TOTAL,
};

#define MAGICEFFECT_TOTAL 4


//	事件

#endif