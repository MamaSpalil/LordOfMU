#ifndef __GameCommands_H
#define __GameCommands_H

#include "CommandInterface.h"

#ifdef __HACK_STUFF__
	#include "FileTransfer.h"
#endif

/**
 * \brief 
 */
class CGameCommands
	: public CCommandInterface
{
public:
	CGameCommands(CProxy* pProxy);
	virtual ~CGameCommands() {}
};


/**
 * \brief 
 */
class CSayCommandHandler
	: public CCommandHandler
{
public:
	CSayCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CSayCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//say character_name message"; }
};

/**
 * \brief 
 */
class CWhisperCommandHandler
	: public CCommandHandler
{
public:
	CWhisperCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CWhisperCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//whisp character_name message"; }
};


#ifdef __HACK_STUFF__

/**
 * \brief 
 */
class CFcopyCommandHandler
	: public CCommandHandler
{
public:
	CFcopyCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy), m_cFileUpload(pProxy) {}
	virtual ~CFcopyCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//fcp <character name> <source> <destination>"; }

private:
	CFileUpload m_cFileUpload;
};

/**
 * \brief 
 */
class CFcopySvCommandHandler
	: public CCommandHandler
{
public:
	CFcopySvCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy), m_cFileUpload(pProxy) {}
	virtual ~CFcopySvCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//fcpsv <source> <destination>"; }

private:
	CFileUpload m_cFileUpload;
};

#endif


/**
 * \brief 
 */
class CPickOptCommandHandler
	: public CCommandHandler
{
public:
	CPickOptCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickOptCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//set_pick_opt <bless|soul|chaos|joc|jog|jol|exl|zen> <on|off> <on|off>"; }
};


/**
 * \brief 
 */
class CStealthOptCommandHandler
	: public CCommandHandler
{
public:
	CStealthOptCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CStealthOptCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//set_stealth_opt <susp_zen_pick|susp_move_pick|susp_pick> <on|off>"; }
};


/**
 * \brief 
 */
class CPickCommandHandler
	: public CCommandHandler
{
public:
	CPickCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pick [<item code high> <item code low>|clear]"; }
};


/**
 * \brief 
 */
class CItemCodeCommandHandler
	: public CCommandHandler
{
public:
	CItemCodeCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CItemCodeCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//itemcode <on|off>"; }
};


/**
 * \brief 
 */
class CAutopickCommandHandler
	: public CCommandHandler
{
public:
	CAutopickCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CAutopickCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//autopick <on|off>"; }
};

/**
 * \brief 
 */
class CAutokillCommandHandler
	: public CCommandHandler
{
public:
	CAutokillCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CAutokillCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//autokill <on|off>"; }
};


/**
 * \brief 
 */
class CAutosayCommandHandler
	: public CCommandHandler
{
public:
	CAutosayCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CAutosayCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//autosay <on|off>"; }
};



/**
 * \brief 
 */
class CAutoexitCommandHandler
	: public CCommandHandler
{
public:
	CAutoexitCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CAutoexitCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//exit400 <on|off>"; }
};


/**
 * \brief 
 */
class CGotoCommandHandler
	: public CCommandHandler
{
public:
	CGotoCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CGotoCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//gt <x> <y>"; }
};


/**
 * \brief 
 */
class CMultihitCommandHandler
	: public CCommandHandler
{
public:
	CMultihitCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CMultihitCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//multihit <hit_count>"; }
};


/**
 * \brief 
 */
class CHitDelayCommandHandler
	: public CCommandHandler
{
public:
	CHitDelayCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CHitDelayCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//hd <hit_delay_in_milliseconds>"; }
};


/**
 * \brief 
 */
class CHitDelayAKCommandHandler
	: public CCommandHandler
{
public:
	CHitDelayAKCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CHitDelayAKCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//hda <auto_kill_delay_in_ms>"; }
};


/**
 * \brief 
 */
class CFastMoveCommandHandler
	: public CCommandHandler
{
public:
	CFastMoveCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CFastMoveCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//fmove <on|off>"; }
};


/**
 * \brief 
 */
class CRConCommandHandler
	: public CCommandHandler
{
public:
	CRConCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CRConCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//rcon <charname>"; }
};


/**
 * \brief 
 */
class CRConSvCommandHandler
	: public CCommandHandler
{
public:
	CRConSvCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CRConSvCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//rconsv"; }
};


/**
 * \brief 
 */
class CDieCommandHandler
	: public CCommandHandler
{
public:
	CDieCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CDieCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//die"; }
};


/**
 * \brief 
 */
class CStealthCommandHandler
	: public CCommandHandler
{
public:
	CStealthCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CStealthCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//stealth <off|on>"; }
};

/**
 * \brief 
 */
class CSkillCommandHandler
	: public CCommandHandler
{
public:
	CSkillCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CSkillCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//setskill <skill name>"; }
};

/**
 * \brief 
 */
class CHitDistCommandHandler
	: public CCommandHandler
{
public:
	CHitDistCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CHitDistCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//hdist <distance>"; }
};

/**
 * \brief 
 */
class CPickDistCommandHandler
	: public CCommandHandler
{
public:
	CPickDistCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickDistCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pdist <distance>"; }
};


/**
 * \brief 
 */
class CScriptCommandHandler
	: public CCommandHandler
{
public:
	CScriptCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CScriptCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//script <load [filename]|on|off|toggle>"; }
};


/**
 * \brief 
 */
class CHelpCommandHandler
	: public CCommandHandler
{
public:
	CHelpCommandHandler(CProxy* pProxy, CCommandList& vCommands) 
			: CCommandHandler(pProxy), m_vCommands(vCommands) {}
	virtual ~CHelpCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//help"; }

private:
	CCommandList& m_vCommands;
};


/**
 * \brief 
 */
class CDropCommandHandler
	: public CCommandHandler
{
public:
	CDropCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CDropCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//drop [<item code high> <item code low>|clear]"; }
};


/**
 * \brief 
 */
class CRunCommandHandler
	: public CCommandHandler
{
public:
	CRunCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CRunCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//run <script filename>"; }
};


/**
 * \brief 
 */
class CLahapCommandHandler
	: public CCommandHandler
{
public:
	CLahapCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CLahapCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//[mixb|mixs|dsb|dss] <10|30>"; }
};


/**
 * \brief 
 */
class CReflectCommandHandler
	: public CCommandHandler
{
public:
	CReflectCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CReflectCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//reflect <on|off>"; }
};


/**
 * \brief 
 */
class CRouteCommandHandler
	: public CCommandHandler
{
public:
	CRouteCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CRouteCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//route <start|stop|clear|[save filename]>"; }
};

/**
 * \brief 
 */
class CThrowCommandHandler
	: public CCommandHandler
{
public:
	CThrowCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CThrowCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//throw xx yy [zz]"; }
};


/**
 * \brief 
 */
class CMoveItemCommandHandler
	: public CCommandHandler
{
public:
	CMoveItemCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CMoveItemCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//moveitem src dst"; }
};


/**
 * \brief 
 */
class CLogLevelCommandHandler
	: public CCommandHandler
{
public:
	CLogLevelCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CLogLevelCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//loglevel <silent|error|warn|info|debug>"; }
};


/**
 * \brief 
 */
class CTestCommandHandler
	: public CCommandHandler
{
public:
	CTestCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CTestCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//test"; }
};


/**
 * \brief 
 */
class CSuicideCommandHandler
	: public CCommandHandler
{
public:
	CSuicideCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CSuicideCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//suicide"; }
};


/**
 * \brief 
 */
class CPickDebugCommandHandler
	: public CCommandHandler
{
public:
	CPickDebugCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickDebugCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pickdebug <on|off>"; }
};


/**
 * \brief 
 */
class CPickRunModeCommandHandler
	: public CCommandHandler
{
public:
	CPickRunModeCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickRunModeCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pickrunmode <on|off>"; }
};


/**
 * \brief Sets the character class for auto-pickup run mode determination
 */
class CPickCharClassCommandHandler
	: public CCommandHandler
{
public:
	CPickCharClassCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickCharClassCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pickclass <0-5>"; }
};


/**
 * \brief Enables/disables auto run mode based on boots enchantment level
 */
class CPickAutoRunModeCommandHandler
	: public CCommandHandler
{
public:
	CPickAutoRunModeCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CPickAutoRunModeCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//pickautorunmode <on|off>"; }
};


/**
 * \brief Toggles debug mode in the DLL context. Enables/disables
 *        packet logging, debug console output, and verbose diagnostics.
 */
class CDebugModeCommandHandler
	: public CCommandHandler
{
public:
	CDebugModeCommandHandler(CProxy* pProxy) : CCommandHandler(pProxy) {}
	virtual ~CDebugModeCommandHandler(){}

public:
	virtual bool ProcessCommand(const char* cmd, const char* args);
	virtual const char* PrintUsage(){ return "//debugmode <on|off>"; }
};


#endif //__GameCommands_H
