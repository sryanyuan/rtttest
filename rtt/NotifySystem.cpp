#include "NotifySystem.h"
#include <Shlwapi.h>
#include <list>
//////////////////////////////////////////////////////////////////////////
NotifySystem::NotifySystem()
{

}

bool NotifySystem::Initialize(const char* _pszConfigFile)
{
	if(!PathFileExists(_pszConfigFile))
	{
		return false;
	}

	char szSectionNames[1024] = {0};
	GetPrivateProfileSectionNames(szSectionNames, sizeof(szSectionNames), _pszConfigFile);

	if(0 == szSectionNames[0])
	{
		return false;
	}

	//	first one
	std::list<const char*> xCCharPtrList;
	xCCharPtrList.push_back(szSectionNames);

	for(int i = 0; i < sizeof(szSectionNames) - 2; ++i)
	{
		if(szSectionNames[i] == 0 &&
			szSectionNames[i + 1] == 0)
		{
			break;
		}

		if(szSectionNames[i] == 0)
		{
			xCCharPtrList.push_back(&szSectionNames[i] + 1);
		}
	}

	if(!xCCharPtrList.empty())
	{
		std::list<const char*>::const_iterator begIter = xCCharPtrList.begin();
		std::list<const char*>::const_iterator endIter = xCCharPtrList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			const char* pszSection = *begIter;

			if(NULL == pszSection ||
				0 == pszSection[0])
			{
				continue;
			}

			NotifyItem item;
			char szMsgBuf[MAX_PATH] = {0};
			GetPrivateProfileString(pszSection, "message", "", szMsgBuf, sizeof(szMsgBuf), _pszConfigFile);
			if(szMsgBuf[0] == 0)
			{
				continue;
			}

			item.xNotification = szMsgBuf;
			item.dwNotifyInterval = GetPrivateProfileInt(pszSection, "interval", 5000, _pszConfigFile);
			item.bLooped = GetPrivateProfileInt(pszSection, "loop", 0, _pszConfigFile) != 0;
			item.dwColor = GetPrivateProfileInt(pszSection, "color", 0xffffffff, _pszConfigFile);

			m_xNotifyItemVector.push_back(item);
		}
	}

	return true;
}

int NotifySystem::QueryNotifyItem(CCharPtrVector& _refCCharVector)
{
	int nQuerySize = 0;

	if(m_xNotifyItemVector.empty())
	{
		return 0;
	}

	for(int i = 0; i < m_xNotifyItemVector.size(); ++i)
	{
		if(GetTickCount() > m_xNotifyItemVector[i].dwPrevNotifyTime &&
			m_xNotifyItemVector[i].bEnabled)
		{
			_refCCharVector.push_back(m_xNotifyItemVector[i].xNotification.c_str());
			m_xNotifyItemVector[i].dwPrevNotifyTime = GetTickCount();

			if(!m_xNotifyItemVector[i].bLooped)
			{
				m_xNotifyItemVector[i].bEnabled = false;
			}

			++nQuerySize;
		}
	}

	return nQuerySize;
}