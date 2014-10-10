#ifndef _INC_OFFLINESELLSYSTEM_
#define _INC_OFFLINESELLSYSTEM_
//////////////////////////////////////////////////////////////////////////
#include "GameRenderObject.h"
#include "sqlite3.h"
#include <vector>
#include <map>
#include <list>
//////////////////////////////////////////////////////////////////////////
#define MAX_SELL_PERPLAYER		8
//////////////////////////////////////////////////////////////////////////
struct SellItem
{
	int nItemID;
	int nUID;
	int nMoney;
	int nGold;
	char szSellerName[20];
	ItemAttrib stAttrib;

	SellItem()
	{
		nItemID = nUID = nMoney = nGold = 0;
		ZeroMemory(szSellerName, sizeof(szSellerName));
		ZeroMemory(&stAttrib, sizeof(ItemAttrib));
	}
};
typedef std::map<int, SellItem> SellItemMap;

struct SoldItem
{
	int nItemID;
	int nUID;
	int nMoney;
	int nGold;
	char szItemName[20];
	char szBuyerName[20];

	SoldItem()
	{
		nItemID = nUID = nMoney = nGold = 0;
		ZeroMemory(szItemName, sizeof(szItemName));
		ZeroMemory(szBuyerName, sizeof(szBuyerName));
	}
};
typedef std::list<SoldItem> SoldItemList;
//////////////////////////////////////////////////////////////////////////
class OfflineSellSystem
{
public:
	~OfflineSellSystem();
protected:
	OfflineSellSystem();
public:
	static OfflineSellSystem* GetInstance()
	{
		static OfflineSellSystem s_ins;
		return &s_ins;
	}

public:
	bool Initialize(const char* _pszSqlFile);
	void Uninitialize();

	bool AddSellItem(int _nUID, int _nMoney, int _nGold, const char* _pszSellerName, const ItemAttrib* _pItem);
	bool RemoveSellItem(int _nItemID);
	const SellItem* GetSellItem(int _nItemID);

	bool AddSoldItem(int _nUID, int _nMoney, int _nGold, const char* _pszItemName, const char* _pszBuyerName);
	bool RemoveSoldItem(int _nItemID);
	void QuerySoldItem(int _nUID, SoldItemList& _refItems);

	bool CopyFromSQL();

protected:
	sqlite3* m_pSQL;
	SellItemMap m_xSellItemMap;
	SoldItemList m_xSoldItemList;
};
//////////////////////////////////////////////////////////////////////////
#endif