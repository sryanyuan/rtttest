#include "OfflineSellSystem.h"
//////////////////////////////////////////////////////////////////////////
OfflineSellSystem::OfflineSellSystem()
{
	m_pSQL = NULL;
}

OfflineSellSystem::~OfflineSellSystem()
{
	Uninitialize();
}

bool OfflineSellSystem::Initialize(const char* _pszSqlFile)
{
	DWORD dwFileSize = 0;

	HANDLE hFile = CreateFile(_pszSqlFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		int nErrCode = GetLastError();

		if(ERROR_FILE_NOT_FOUND != nErrCode)
		{
			Uninitialize();
			return false;
		}
	}
	else
	{
		dwFileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	int nRet = sqlite3_open(_pszSqlFile, &m_pSQL);
	if(SQLITE_OK != nRet)
	{
		return false;
	}

	if(0 == dwFileSize)
	{
		//	create tbl_sellitems table
		const char* pszTableExpr = "create table tbl_sellitems("\
			"id integer primary key,"\
			"seller_uid int,"\
			"money int,"\
			"gold int,"\
			"seller_name varchar(20),"\
			"item_data blob"\
			")";
		char* szErr = NULL;
		nRet = sqlite3_exec(m_pSQL, pszTableExpr, NULL, NULL, &szErr);
		if(SQLITE_OK != nRet)
		{
			sqlite3_free(szErr);
			Uninitialize();
			return false;
		}

		//	create tbl_solditems table
		pszTableExpr = "create table tbl_solditems("\
			"id integer primary key,"\
			"seller_uid int,"\
			"money int,"\
			"gold int,"\
			"item_name varchar(20),"\
			"buyer_name varchar(20)"\
			")";
		nRet = sqlite3_exec(m_pSQL, pszTableExpr, NULL, NULL, &szErr);
		if(SQLITE_OK != nRet)
		{
			sqlite3_free(szErr);
			Uninitialize();
			return false;
		}
	}

	return true;
}

void OfflineSellSystem::Uninitialize()
{
	if(m_pSQL)
	{
		sqlite3_close(m_pSQL);
		m_pSQL = NULL;
	}
}

bool OfflineSellSystem::AddSellItem(int _nUID, int _nMoney, int _nGold, const char* _pszSellerName, const ItemAttrib* _pItem)
{
	char szSqlExpr[1024];
	char szSerializeBuf[100] = {0};
	
	//	serialize
	memcpy(szSerializeBuf, _pItem, sizeof(ItemAttrib));

	//	insert...
	sqlite3_stmt* pStmt = NULL;
	bool bRet = false;
	int nRet = 0;

	do 
	{
		sprintf(szSqlExpr, "select count(seller_uid) from tbl_sellitems where seller_uid=%d",
			_nUID);
		nRet = sqlite3_prepare(m_pSQL, szSqlExpr, -1, &pStmt, NULL);
		if(SQLITE_OK != nRet)
		{
			break;
		}
		nRet = sqlite3_step(pStmt);
		if(SQLITE_ROW != nRet)
		{
			break;
		}
		int nRowCount = sqlite3_column_int(pStmt, 0);
		if(nRowCount > MAX_SELL_PERPLAYER)
		{
			break;
		}
		bRet = true;
	} while (0);

	if(pStmt)
	{
		sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	if(!bRet)
	{
		return false;
	}

	bRet = false;
	//	sql expression
	sprintf(szSqlExpr, "insert into tbl_sellitems (seller_uid, money, gold, seller_name, item_data) values(%d, %d, %d, '%s', ?)",
		_nUID, _nMoney, _nGold, _pszSellerName);

	do 
	{
		nRet = sqlite3_prepare(m_pSQL, szSqlExpr, -1, &pStmt, NULL);
		if(SQLITE_OK != nRet ||
			pStmt == NULL)
		{
			break;
		}
		nRet = sqlite3_bind_blob(pStmt, 1, szSerializeBuf, sizeof(ItemAttrib), NULL);
		if(SQLITE_OK != nRet)
		{
			break;
		}
		nRet = sqlite3_step(pStmt);
		if(SQLITE_DONE != nRet)
		{
			break;
		}

		bRet = true;
	} while (0);

	if(pStmt)
	{
		sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	if(!bRet)
	{
		return false;
	}

	//	get item id
	int nItemID = sqlite3_last_insert_rowid(m_pSQL);

	SellItem item;
	item.nItemID = nItemID;
	item.nMoney = _nMoney;
	item.nGold = _nGold;
	item.nUID = _nUID;
	strcpy(item.szSellerName, _pszSellerName);
	memcpy(&item.stAttrib, _pItem, sizeof(ItemAttrib));
	m_xSellItemMap.insert(std::make_pair(nItemID, item));

	return bRet;
}

const SellItem* OfflineSellSystem::GetSellItem(int _nItemID)
{
	SellItemMap::const_iterator fndIter = m_xSellItemMap.find(_nItemID);
	if(fndIter != m_xSellItemMap.end())
	{
		return &fndIter->second;
	}

	return NULL;
}

bool OfflineSellSystem::RemoveSellItem(int _nItemID)
{
	SellItem item;
	SellItemMap::iterator fndIter = m_xSellItemMap.find(_nItemID);
	if(fndIter == m_xSellItemMap.end())
	{
		return false;
	}
	else
	{
		item = fndIter->second;
		m_xSellItemMap.erase(fndIter);
	}

	char szSqlExpr[1024];
	sprintf(szSqlExpr, "delete from tbl_sellitems where id=%d",
		item.nItemID);
	char* pErr = NULL;
	int nRet = sqlite3_exec(m_pSQL, szSqlExpr, NULL, NULL, &pErr);
	if(SQLITE_OK != nRet)
	{
		return false;
	}

	return true;
}

bool OfflineSellSystem::AddSoldItem(int _nUID, int _nMoney, int _nGold, const char* _pszItemName, const char* _pszBuyerName)
{
	char szSqlExpr[1024];

	//	sql expression
	sprintf(szSqlExpr, "insert into tbl_solditems (seller_uid,money,gold,item_name,buyer_name) values("\
		"%d,"\
		"%d,"\
		"%d,"\
		"'%s',"\
		"'%s'"\
		")",
		_nUID,
		_nMoney,
		_nGold,
		_pszItemName,
		_pszBuyerName);

	char* szErr = NULL;
	int nRet = sqlite3_exec(m_pSQL, szSqlExpr, NULL, NULL, &szErr);
	if(SQLITE_OK != nRet)
	{
		sqlite3_free(szErr);
		return false;
	}

	SoldItem item;
	item.nItemID = sqlite3_last_insert_rowid(m_pSQL);
	item.nUID = _nUID;
	item.nMoney = _nMoney;
	item.nGold = _nGold;
	strcpy(item.szBuyerName, _pszBuyerName);
	strcpy(item.szItemName, _pszItemName);
	m_xSoldItemList.push_back(item);

	return true;
}

void OfflineSellSystem::QuerySoldItem(int _nUID, SoldItemList& _refItems)
{
	SoldItemList::const_iterator begIter = m_xSoldItemList.begin();
	SoldItemList::const_iterator endIter = m_xSoldItemList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		const SoldItem& refItem = *begIter;

		if(refItem.nUID == _nUID)
		{
			_refItems.push_back(refItem);
		}
	}
}

bool OfflineSellSystem::RemoveSoldItem(int _nItemID)
{
	SoldItemList::iterator begIter = m_xSoldItemList.begin();
	SoldItemList::const_iterator endIter = m_xSoldItemList.end();

	SoldItem item;

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		SoldItem& refItem = *begIter;

		if(refItem.nItemID == _nItemID)
		{
			item = refItem;
			m_xSoldItemList.erase(begIter);
			break;
		}
	}

	if(item.nItemID == 0)
	{
		return false;
	}

	char szSqlExpr[1024];
	sprintf(szSqlExpr, "delete from tbl_solditems where id=%d",
		item.nItemID);
	char* pErr = NULL;
	int nRet = sqlite3_exec(m_pSQL, szSqlExpr, NULL, NULL, &pErr);
	if(SQLITE_OK != nRet)
	{
		return false;
	}

	return true;
}

bool OfflineSellSystem::CopyFromSQL()
{
	if(NULL == m_pSQL)
	{
		return false;
	}

	//	get all sellitems
	sqlite3_stmt* stmt = NULL;
	bool bRet = false;

	do 
	{
		int nRet = sqlite3_prepare(m_pSQL, "select * from tbl_sellitems", -1, &stmt, NULL);

		if(SQLITE_OK != nRet ||
			stmt == NULL)
		{
			break;
		}
		nRet = sqlite3_step(stmt);
		for(;;)
		{
			if(nRet != SQLITE_ROW)
			{
				break;
			}

			//	read
			SellItem item;
			item.nItemID = sqlite3_column_int(stmt, 0);
			item.nUID = sqlite3_column_int(stmt, 1);
			item.nMoney = sqlite3_column_int(stmt, 2);
			item.nGold = sqlite3_column_int(stmt, 3);
			const unsigned char* pszText = sqlite3_column_text(stmt, 4);
			if(NULL != pszText)
			{
				strcpy(item.szSellerName, (const  char*)pszText);
			}
			unsigned int uDataLen = sqlite3_column_bytes(stmt, 5);
			const void* pData = sqlite3_column_blob(stmt, 5);
			//	unserialize
			memcpy(&item.stAttrib, pData, uDataLen);

			if(item.nItemID != 0)
			{
				m_xSellItemMap.insert(std::make_pair(item.nItemID, item));
			}

			nRet = sqlite3_step(stmt);
		}
		bRet = true;
	} while (0);

	if(stmt)
	{
		sqlite3_finalize(stmt);
		stmt = NULL;
	}

	if(!bRet)
	{
		return false;
	}

	/*
	"create table tbl_solditems("\
	"id integer primary key,"\
	"seller_uid int,"\
	"money int,"\
	"gold int,"\
	"item_name varchar(20),"\
	"buyer_name varchar(20)"\
	")";
	*/
	bRet = false;
	do 
	{
		int nRet = sqlite3_prepare(m_pSQL, "select * from tbl_solditems", -1, &stmt, NULL);

		if(SQLITE_OK != nRet ||
			stmt == NULL)
		{
			break;
		}
		nRet = sqlite3_step(stmt);
		for(;;)
		{
			if(nRet != SQLITE_ROW)
			{
				break;
			}

			//	read
			SoldItem item;
			item.nItemID = sqlite3_column_int(stmt, 0);
			item.nUID = sqlite3_column_int(stmt, 1);
			item.nMoney = sqlite3_column_int(stmt, 2);
			item.nGold = sqlite3_column_int(stmt, 3);
			const unsigned char* pszText = sqlite3_column_text(stmt, 4);
			if(NULL != pszText)
			{
				strcpy(item.szItemName, (const  char*)pszText);
			}
			pszText = sqlite3_column_text(stmt, 4);
			if(NULL != pszText)
			{
				strcpy(item.szBuyerName, (const  char*)pszText);
			}

			if(item.nItemID != 0)
			{
				m_xSoldItemList.push_back(item);
			}

			nRet = sqlite3_step(stmt);
		}
		bRet = true;
	} while (0);

	if(stmt)
	{
		sqlite3_finalize(stmt);
		stmt = NULL;
	}

	if(!bRet)
	{
		return false;
	}

	return true;
}