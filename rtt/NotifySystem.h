#ifndef _INC_NOTIFYSYSTEM_
#define _INC_NOTIFYSYSTEM_
//////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////
typedef std::vector<const char*> CCharPtrVector;

struct NotifyItem
{
	string xNotification;
	DWORD dwNotifyInterval;
	DWORD dwPrevNotifyTime;
	DWORD dwColor;
	bool bLooped;
	bool bEnabled;

	NotifyItem()
	{
		dwNotifyInterval = 5000;
		dwPrevNotifyTime = 0;
		bLooped = false;
		bEnabled = true;
		dwColor = 0xffffffff;
	}
};

typedef std::vector<NotifyItem> NotifyItemVector;
//////////////////////////////////////////////////////////////////////////
class NotifySystem
{
public:
	~NotifySystem(){}
protected:
	NotifySystem();
public:
	static NotifySystem* GetInstance()
	{
		static NotifySystem s_ins;
		return &s_ins;
	}

public:
	bool Initialize(const char* _pszConfigFile);
	int QueryNotifyItem(CCharPtrVector& _refCCharVector);

protected:
	NotifyItemVector m_xNotifyItemVector;
};
//////////////////////////////////////////////////////////////////////////
#endif