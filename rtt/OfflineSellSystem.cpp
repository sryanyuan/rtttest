#include "OfflineSellSystem.h"
#
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
			"id int primary key,"\
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
			"id int primary key,"\
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
	/*
	"create table tbl_sellitems("\
	"id int primary key,"\
	"seller_uid int,"\
	"money int,"\
	"gold int,"\
	"seller_name varchar(20),"\
	"item_data varchar(100)"\
	")";
	*/
	
	//	serialize
	memcpy(szSerializeBuf, _pItem, sizeof(ItemAttrib));

	//	sql expression
	sprintf(szSqlExpr, "insert into tbl_sellitems (seller_uid, money, gold, seller_name, item_data) values(%d, %d, %d, '%s', ?)",
		_nUID, _nMoney, _nGold, _pszSellerName);

	//	insert...
	sqlite3_stmt* pStmt = NULL;
	bool bRet = false;
	int nRet = 0;

	do 
	{
		nRet = sqlite3_prepare(m_pSQL, szSqlExpr, -1, &pStmt, NULL);
		if(SQLITE_OK != nRet)
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
		nRet = sqlite3_finalize(pStmt);
		if(SQLITE_OK != nRet)
		{
			break;
		}

		bRet = true;
	} while (0);

	return bRet;
}

bool OfflineSellSystem::AddSoldItem(int _nUID, int _nMoney, int _nGold, const char* _pszItemName, const char* _pszBuyerName)
{
	/*
	"create table tbl_solditems("\
	"id int primary key,"\
	"seller_uid int,"\
	"money int,"\
	"gold int,"\
	"item_name varchar(20),"\
	"buyer_name varchar(20)"\
	")";
	*/

	char szSqlExpr[1024];

	//	sql expression
	sprintf(szSqlExpr, "insert into tbl_solditems (seller_uid,money,gold,item_name,buyer_name) values("\
		"%d,"\
		"%d,"\
		"%d,"\
		"%s,"\
		"%s"\
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

	return true;
}