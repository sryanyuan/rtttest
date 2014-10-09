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

//	����
#define ITEM_NO			0
//	��
#define ITEM_BOOK		1
//	�·�
#define ITEM_CLOTH		2
//	����
#define ITEM_NECKLACE	3
//	����
#define ITEM_BRACELAT	4
//	��ָ
#define ITEM_RING		5
//	ѫ��
#define ITEM_MEDAL		6
//	ͷ��
#define ITEM_HELMET		7
//	ҩƷ
#define ITEM_YAO_SPE	8
//	����
#define ITEM_WEAPON		9
//	����
#define ITEM_OTHER		10
//	��ͨҩƷ ����ʹ�õ���Ʒ(ҩƷ ����)
#define ITEM_COST		11
//	��
#define ITEM_BALE		12
//	����
#define ITEM_SCROLL		13
//	Ь��
#define ITEM_SHOE		14
//	����
#define ITEM_BELT		15
//	��ʯ
#define ITEM_GEM		16
//	����
#define ITEM_CHARM		17
//	���ڲ�ѯ
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

//	��������
#define MAIN_BAG_SIZE	36
#define ASSIST_BAG_SIZE	6
//	�������� Weightֵ
#define WEIGHT_MASK_MGCATK	0x01
#define WEIGHT_MASK_SEEHIDE	0x02
#define WEIGHT_MASK_UNMOVE	0x04
#define WEIGHT_MASK_DEFFIRE	0x08
#define WEIGHT_MASK_STONE	0x10
#define WEIGHT_MASK_POISON	0x20
#define WEIGHT_MASK_IGNOREAC	0x40
#define WEIGHT_MASK_DEFICE	0x80
//	װ������ Poisֵ
#define POIS_MASK_BIND		0x01
//	��������

//	װ������ Exprֵ
#define EXPR_MASK_NOSAVE	0x00000001
#define EXPR_MASK_NOSELL	0x00000002
//	�������� MaxExprֵ
#define MAXEXPR_MASK_BOSS		0x00000001
#define MAXEXPR_MASK_DEADHIDE	0x00000002
#define MAXEXPR_MASK_ELITE		0x00000004
#define MAXEXPR_MASK_LEADER		0x00000008

//	������� ֻ������
#define TEX_MASK_HIDE		0x00000001

#define EXTRA_MASK_ENCRYPT	0x0001

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

struct GroundItem
{
	ItemAttrib stAttrib;
	WORD wPosX;
	WORD wPosY;
	//	��Ӧ�����������ɵ���ƷID
	DWORD wID;
};

typedef std::vector<ItemAttrib> ItemList;
typedef std::list<GroundItem*>	GROUNDITEMS;

//	������Ϣ
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

//	���װ����Ϣ

struct PlayerEquipAttrib
{
	//	�������
	int nWeapon;
	//	����·�
	int nCloth;
	//	�������
	int nNeck;
	//	�������
	int nBrac[2];
	//	��ҽ�ָ
	int nRing[2];
	//	���ͷ��
	int nHelm;
	//	���ѫ��
	int nMedal;
	//	��ұ�ʯ
	int nGem;
	//	���Ь��
	int nShoe;
	//	�������
	int nBelt;
	//	��ҷ���
	int nHairStyle;
};

//	װ��������Ϣ
struct PlayerTexIndexTable
{
	//	��������
	USHORT wHum;
	//	����
	USHORT wHair;
	//	����
	USHORT wWeapon;
};

//	��ͼ����װ����Ϣ
class GameObject;
struct GroundItem;
class MagicElement;
#define MAX_CELL_OBJECTS	5
struct MemMapObject
{
	//	����
	GameObject* pObjects[MAX_CELL_OBJECTS];
	//	װ��
	GroundItem* pItems[MAX_CELL_OBJECTS];
	//	ħ��
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

//	ħ��Ч��
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


//	�¼�

#endif