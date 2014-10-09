#ifndef _INC_OFFLINESELLSYSTEM_
#define _INC_OFFLINESELLSYSTEM_
//////////////////////////////////////////////////////////////////////////
#include "GameRenderObject.h"
#include "sqlite3.h"
#include <vector>
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
	bool GetSellItems();
	bool AddSoldItem(int _nUID, int _nMoney, int _nGold, const char* _pszItemName, const char* _pszBuyerName);

protected:
	sqlite3* m_pSQL;
};
//////////////////////////////////////////////////////////////////////////
#endif