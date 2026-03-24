#include "stdafx.h"
#include "ProxyBuilder.h"
#include "GameProxy.h"
#include "PassThroughProxy.h"
#include "PacketLogger.h"
#include "PacketDecryptFilter.h"
#include "PacketEncryptFilter.h"
#include "GameCommands.h"
#include "DebugOut.h"
#include "AutoPickupFilter.h"
#include "AutosayFilter.h"
#include "CharInfoFilter.h"
#include "MultihitPacketFilter.h"
#include "FastMoveFilter.h"
#include "AutoKillFilter.h"
#include "ScriptProcessorFilter.h"
#include "ReflectPacketFilter.h"
#include "RecordRoutePacketFilter.h"
#include "InventoryManagerFilter.h"


/**
 * \brief 
 */
CProxy* CProxyBuilder::CreateProxy(SOCKET s, const struct sockaddr* addr, int len)
{
	if (s != 0 && len == sizeof(sockaddr_in))
	{
		sockaddr_in* addr_in = (sockaddr_in*)addr;

		WORD port = MAKEWORD(HIBYTE(addr_in->sin_port), LOBYTE(addr_in->sin_port));

		if (port > 44000 && port < 50000)
		{
			// assume it is the connect server
			// CDebugOut::PrintDebug("Creating bypass proxy ...");
			return 0; //CreateBypassProxy(s);
		}
		else
		{
			CDebugOut::PrintDebug("Creating game proxy ...");
			return CreateGameProxy(s);
		}
	}
	
	CDebugOut::PrintWarn("Creating proxy failed!");
	return 0;
}

/**
* \brief 
*/
CProxy* CProxyBuilder::CreateBypassProxy(SOCKET s)
{
	return new CPassThroughProxy(s);
}

/**
* \brief 
*/
CProxy* CProxyBuilder::CreateGameProxy(SOCKET s)
{
	CGameProxy* pProxy = new CGameProxy(s);

	pProxy->AddFilter(new CPacketDecryptFilter(pProxy));
	pProxy->AddFilter(new CPacketLogger(pProxy));

#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
	pProxy->AddFilter(new CCharInfoFilter(pProxy));
	pProxy->AddFilter(new CGameCommands(pProxy));
	pProxy->AddFilter(new CAutoPickupFilter(pProxy));
	pProxy->AddFilter(new CAutosayFilter(pProxy));
	pProxy->AddFilter(new CScriptProcessorFilter(pProxy));
#endif

#if defined(__HACK_STUFF__) || defined(DEBUG) || defined(__INCLUDE_ALL_STUFF__)
	pProxy->AddFilter(new CAutoKillFilter(pProxy));
	pProxy->AddFilter(new CMultihitPacketFilter(pProxy));
	pProxy->AddFilter(new CFastMoveFilter(pProxy));
	pProxy->AddFilter(new CReflectPacketFilter(pProxy));
	pProxy->AddFilter(new CRecordRoutePacketFilter(pProxy));
	pProxy->AddFilter(new CInventoryManagerFilter(pProxy));
#endif

	pProxy->AddFilter(new CPacketEncryptFilter(pProxy));

	return pProxy;
}

