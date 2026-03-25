#include "stdafx.h"
#include "AntiCheat.h"

#define TitleLen 1024
#define BadWordsCount 128

typedef struct Anti_BadWords 
{
	char BadWords[100] ;
};

typedef struct Word_Parts 
{
	char Parts[1024] ;
};

Word_Parts Word_Part[20]={0};
Anti_BadWords Anti_BadWord [BadWordsCount] = 
{
////////////////////////////////////////////////////////////////////////////////////////////////
//Heuristic scan - HNC-Scan (Bad words) 
//Are Case-sensitive - Dont scan Web Brosers
//{{"xxx"}}, 
////////////////////////////////////////////////////////////////////////////////////////////////

	//NEVER FORGOT TO UPDATE BadWordsCount ABOVE

	{{"autocliker"}},
	{{"elite"}},
	{{"mupie"}},
	{{"classname"}},
	{{"bot"}},
	{{"zhypermu"}},
	{{"catastrophe"}},
	{{"muhackrm"}},
	{{"topgun"}},
	{{"gmo"}},
	{{"dupehack"}},
	{{"hack"}},
	{{"cheat4fun"}},
	{{"d-c"}},
	{{"gunz"}},
	{{"pie"}},
	{{"godlike"}},
	{{"zmu"}},
	{{"zergnm"}},
	{{"muproxy"}},
	{{"wildproxy"}},
	{{"muwars"}},
	{{"www.cheats.lv"}},
	{{"speedhack"}},
	{{"hasy"}},
	{{"hasty"}},
	{{"hastymu"}},
	{{"hasymu"}},
	{{"j0ker"}},
	{{"joker"}},
	{{"dirtystats"}}, 
	{{"stat"}},
	{{"wildpower"}},
	{{"mupower"}},
	{{"muhp"}},
	{{"magicshield"}}, 
	{{"hidetoolz"}},
	{{"www.sysinternals.com"}},
	{{"wpe"}},
	{{"funnyhacking"}}, 
	{{"cheat"}},
	{{"mucheats"}},
	{{"cheats"}},
	{{"mods"}},
	{{"autoit"}},
	{{"4muwars.lv"}},
	{{"pjautoplay"}},
	{{"happens"}},
	{{"xelerator"}},
	{{"kiasu"}},
	{{"speederxp"}},
	{{"lipsum"}},
	{{"ziomal"}},
	{{"hahamu"}},
	{{"cani"}},
	{{"zmu"}},
	{{"wowtfistderkerlhin?!"}},
	{{"energy"}},
	{{"ollydbg"}},
	{{"tradebug"}},
	{{"uopilot"}},
	{{"mushv2"}},
	{{"blaster"}},
	{{"mu.gate.vn"}},
	{{"muvn"}},
	{{"gm"}}, 
	{{"paulo94"}},
	{{"injector"}},
	{{"hithack"}},
	{{"godmode"}},
	{{"blaster"}},
	{{"shadow"}},
	{{"stacker"}},
	{{"solidproxy"}},
	{{"c0da"}},
	{{"7mm"}},
	{{"gustymu"}},
	{{"kiki's"}},
	{{"kiki"}},
	{{"uce"}},
	{{"cheathappens"}},
	{{"autocombo"}},
	{{"ghostproxy"}},
	{{"wanted"}},
	{{"elitepvpers.de"}},
	{{"combo"}},
	{{"vaultblaster"}},
	{{"charblaster"}},
	{{"moonlight"}},
	{{"faaf"}},
	{{"ilvmoney"}},
	{{"darkystats"}},
	{{"autoclicker"}},
	{{"muautoclicker"}},
	{{"diremu-autobuff"}},
	{{"sbh1000"}},
	{{"solid"}},
	{{"proxy"}},
	{{"snd.bot"}},
	{{"sandin"}},
	{{"wireshark"}},
	{{"kismet"}},
	{{"tcpdump"}},
	{{"ettercap"}},
	{{"dsniff"}},
	{{"sniffer"}},
	{{"netstumbler"}},
	{{"ntop"}},
	{{"ngrep"}},
	{{"kismac"}},
	{{"etherape"}},
	{{"packet"}},
	{{"darkstat"}},
	{{"ethereal"}},
	{{"sniffit"}},
	{{"sandbox"}},
	{{"sandboxed"}},
	{{"hunter"}},
	{{"pchunter"}},
	{{"process"}},
	{{"thread"}},
	{{"dump"}},
	{{"memory"}},
	{{"viewer"}},
	{{"[#]"}},   
	{{"dupe"}},
	{{"?olidpro×?"}},
	{{"1.22"}}
};      //solid proxy
	//NEVER FORGOT TO UPDATE BadWordsCount NUMBER

 
extern Anti_BadWords Anti_BadWord[BadWordsCount] ;
char windowName[TitleLen];
char ClassName[TitleLen];
char * LowCaseChanger (char MixedCase[TitleLen])
{
	int i,WordLen=0;
	WordLen=strlen(MixedCase);
	for (i=0;i<=WordLen;i++)
	{
		if ( MixedCase[i] >= 65 && MixedCase[i] <=90 )
			MixedCase[i]=MixedCase[i] + 32;
	}
	return MixedCase;
}

BOOL CALLBACK MyEnumProc(HWND hWnd, LPARAM lParam)
{
	Word_Parts Word_Part[20]={0};
	int i,j,z,lenwords;
	char *pword,partWindowName[1024]={0} ;
	GetWindowText(hWnd, windowName, sizeof(windowName));	
	GetClassName(hWnd, ClassName, sizeof(ClassName) );
	if (strcmp(windowName,"Default IME" ) == 0) 
	{
		return false;
	}
	strcpy(windowName,LowCaseChanger(windowName));
	strcpy(ClassName,LowCaseChanger(ClassName));
	if(strcmp(ClassName,"tabthumbnailwindow")==0 || strcmp(ClassName,"mozillauiwindowclass")==0 || strcmp(ClassName,"chrome_widgetwin_0")==0 || strcmp(ClassName,"chrome_widgetwin_1")==0 || strcmp(ClassName,"ieframe")==0)	//prevent from scaning internet browsers, 
	{
	return true;
	}
	pword=&windowName[0];			//start Scan Titles
	for (i=0; i<=19 ; i++)
	{
		sscanf(pword,"%s",Word_Part[i].Parts);
		lenwords=strlen(Word_Part[i].Parts) +1;
		pword=pword+lenwords;		            	//move pointer to the next word
		for (j=0;j<=BadWordsCount-1;j++)			//scan badwords 
		{
			if(strcmp(Anti_BadWord[j].BadWords,Word_Part[i].Parts) == 0 || strcmp(Anti_BadWord[j].BadWords,ClassName) == 0)
			{

				LogsAdded ("[Closed]: %s" , windowName);
				ExitProcess(0);					
				return (false);		
			}
		}
		if(lenwords==1)
		break;
	}
	return true;
}

void DOWN_H()
{	
int Scan_Titles(void);
		EnumWindows(MyEnumProc,0);		
}

void UP_Hnc()
{
again:
    DOWN_H();
    Sleep(10000);
    goto again;

}

void Heuristica2(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(UP_Hnc),NULL,0,0);
}
  
//==============================================================================


void TxtCheckWindow()
{
	POINT p;
	HWND DebugerFound = 0;
	for ( int qy = 0 ; qy < 100 ; qy++)	{
	for ( int qx = 0 ; qx < 100 ; qx++)	{
	p.x = qx * 20;
	p.y = qy * 20;	
	DebugerFound = WindowFromPoint(p);
	char t[255];
	GetWindowTextA( DebugerFound , t , 225); 

////////////////////////////////////////////////////////////////////////////////////////////////
//Heuristic scan - H-Scan (Bad words) 
//Are Case-sensitive - scan Web Brosers
//strstr(t,"xxxx")    || 
////////////////////////////////////////////////////////////////////////////////////////////////

if (strstr(t,"Acelera")		||
	strstr(t,"acelera")  	||	
	strstr(t,"Acelerador")	||	
	strstr(t,"acelerador")	||
	strstr(t,"[F?Ø???] [???]")  || //Black box
	strstr(t,"Black Box")   ||
	strstr(t,"BYPASS")	    ||
	strstr(t,"ByPass")		||
	strstr(t,"byPass")		||
	strstr(t,"Bypass")		||
	strstr(t,"CAPOTE")	    ||
	strstr(t,"Capote")	    ||
	strstr(t,"capote")	    ||	
	strstr(t,"CATASTROPHE")	||
	strstr(t,"Catastrophe")	||
	strstr(t,"catastrophe")	||
	strstr(t,"CHEAT")	    ||
	strstr(t,"Cheat")	    ||
	strstr(t,"cheat")	    ||
	strstr(t,"CHEATER")	    ||
	strstr(t,"Cheater")	    ||
	strstr(t,"cheater")	    ||
	strstr(t,"CHEATS")      ||
	strstr(t,"Cheats")      ||
	strstr(t,"cheats")      ||
	strstr(t,"DUPAR")		||
	strstr(t,"Dupar")		||
	strstr(t,"dupar")		||
	strstr(t,"DUPER")		||
	strstr(t,"Duper")		||
	strstr(t,"duper")		||
	strstr(t,"ENGINE")	    ||
	strstr(t,"Engine")	    ||
	strstr(t,"engine")	    ||
	strstr(t,"EXPLORER")	||
	strstr(t,"Explorer")	||
	strstr(t,"explorer")	||
	strstr(t,"PROCESS")	    ||
	strstr(t,"Process")	    ||
	strstr(t,"process")    	||
	strstr(t,"FREEZE")		||
	strstr(t,"Freeze")		||
	strstr(t,"freeze")		||
	strstr(t,"SPEED")       ||
	strstr(t,"Speed")       ||
	strstr(t,"Speed")       ||
	strstr(t,"HACK")		||
	strstr(t,"Hack")		||
	strstr(t,"hack")		||
	strstr(t,"HACKER")		||
	strstr(t,"Hacker")		||
	strstr(t,"hacker")		||
	strstr(t,"HIDE")		||
	strstr(t,"Hide")		||
	strstr(t,"hide")		||
	strstr(t,"TOOLZ")		||
	strstr(t,"Toolz")		||
	strstr(t,"toolz")		||
	strstr(t,"INJECT")	    ||
	strstr(t,"Inject")	    ||
	strstr(t,"inject")	    ||
	strstr(t,"INJECTOR")	||
	strstr(t,"Injector")	||
	strstr(t,"injector")	||
	strstr(t,"KILL")	    ||
	strstr(t,"Kill")		||
	strstr(t,"kill")		||
	strstr(t,"MUPIE")		||
	strstr(t,"MuPie")		||
	strstr(t,"MuPie")		||
	strstr(t,"OLLY")		||
	strstr(t,"Olly")		||
	strstr(t,"olly")		||
	strstr(t,"PACKAGER")	||
	strstr(t,"Packager")	||
	strstr(t,"Packager")	||
	strstr(t,"PACOTES")	    ||
	strstr(t,"Pacotes")	    ||
	strstr(t,"Pacotes")	    ||
	strstr(t,"SUSPEND")		||
	strstr(t,"Suspend")		||
	strstr(t,"suspend")		||
	strstr(t,"WILDPROXY")	||
	strstr(t,"Wildproxy")	||
	strstr(t,"wildproxy")	||
	strstr(t,"WPE PRO")		||
	strstr(t,"Wpe Pro")		||
	strstr(t,"wpe pro")		||
	strstr(t,"XELERATOR")	||
	strstr(t,"Xelerator")	||
	strstr(t,"xelerator")	||	
	strstr(t,"XTRAP")	    ||
	strstr(t,"XTrap")	    ||
	strstr(t,"xtrap")	    ||
	strstr(t,"XSPEED")	    ||
	strstr(t,"XSpeed")	    ||
	strstr(t,"xSpeed")	    ||
	strstr(t,"xspeed")	    ||	
	strstr(t,"FUNNYZHYPER")	||
	strstr(t,"FunnyZhyper")	||
	strstr(t,"funnyzhyper")	||
    strstr(t,"AUTOSKILL")	||
	strstr(t,"AutoSkill")	||
	strstr(t,"autoskill")	||
	strstr(t,"AUTOKILL")	||
	strstr(t,"Autokill")	||
	strstr(t,"autokill")	||
	strstr(t,"AGILITY")		||
	strstr(t,"Agility")		||
	strstr(t,"agility")		||
	strstr(t,"TRADE")	    ||
	strstr(t,"Trade")	    ||
	strstr(t,"trade")	    ||
	strstr(t,"PACKET")	    ||
	strstr(t,"Packet")	    ||
	strstr(t,"packet")	    ||
	strstr(t,"AUTOIT")	    ||
	strstr(t,"AutoIt")	    ||
	strstr(t,"autoit")	    ||
	strstr(t,"AUTOKEYBOARD") ||
	strstr(t,"AutoKeyboard") ||
	strstr(t,"autokeyboard") ||	
	strstr(t,"SMOLL")    	||
	strstr(t,"Smoll")    	||
	strstr(t,"smoll")    	||	
	strstr(t,"SANDBOX")     ||
	strstr(t,"Sandbox")     ||
	strstr(t,"sandbox")     ||
	strstr(t,"SANDBOXED")   ||
	strstr(t,"Sandboxed")   ||
	strstr(t,"sandboxed")   ||
	strstr(t,"DEFALTBOX")   ||
	strstr(t,"DefaltBox")   ||
	strstr(t,"defaltbox")   ||
	strstr(t,"HXD")         ||
	strstr(t,"HxD")         ||
	strstr(t,"hxd")         ||
	strstr(t,"BVKHEX")      ||
	strstr(t,"Bvkhex")      ||
	strstr(t,"bvkhex")      ||
	strstr(t,"bvkhex")      ||
	strstr(t,"bvkhex")      ||
	strstr(t,"PAUSE")       ||
	strstr(t,"Pause")       ||
	strstr(t,"pause")       ||
	strstr(t,"PAUSA")       ||
	strstr(t,"Pausa")       ||
	strstr(t,"pausa")       ||
	strstr(t,"resume")      ||
	strstr(t,"Real Server:")||               //SolidProxy Stealth mode
	strstr(t,"FREE PUBLIC VERSION!")||       //SolidProxy Stealth mode
	strstr(t,"44405")       ||               //SolidProxy Stealth mode
	strstr(t,"55901")       ||               //SolidProxy Stealth mode
	strstr(t,"Connect Server Emulator:")||   //SolidProxy Stealth mode
	strstr(t,"Button Classname Detecta")||
	strstr(t,"AutoClicker") ||
	strstr(t,"Autoclicker") ||
	strstr(t,"autoclicker") ||
	strstr(t,"(Elite)")     ||
	strstr(t,"Elite")       ||
	strstr(t,"Style30_Theme1")  || //darkterro gamehacks
	strstr(t,"DEBUG")       ||
	strstr(t,"DEBUG")       ||
	strstr(t,"Debug")       ||
	strstr(t,"debug"))
	{

		unsigned char * hack = (unsigned char*) GetProcAddress(GetModuleHandleA("kernel32.dll"), "OpenProcess");
		if ( *(hack+6) == 0xEA )
		{ 
			Sleep(1);
		}

		LogsAdded ("[Closed]: %s" , (t));

		ExitProcess(0);
	}
    }
	}
}
	

void H_Scan()
{

again:
    TxtCheckWindow();
    Sleep(10000);
    goto again;

}

void HProtection()
{
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(H_Scan),NULL,0,0);
}
//=====================================================================================

BOOL CALLBACK heurprocess(HWND hwnd, LPARAM lParam)
{
    static TCHAR x[40];
    GetWindowText(hwnd, x, 40);

////////////////////////////////////////////////////////////////////////////////////////////////
//Heuristic Window name scan - HW-scan 
//Are Case-sensitive - Find it just looking window name or using [Handler 1.5 by Oliver Bock] 
//_tcsstr(x,"xxx") Just one word, without empty space
////////////////////////////////////////////////////////////////////////////////////////////////

if (_tcsstr(x,"Acelera")	||
	_tcsstr(x,"acelera")  	||	
	_tcsstr(x,"autopot")  	||
	_tcsstr(x,"Optimus Prime")  ||	
	_tcsstr(x,"H4ck3ker")  	||
	_tcsstr(x,"privado")  	||	
	_tcsstr(x,"Acelerador")	||	
	_tcsstr(x,"acelerador")	||
	_tcsstr(x,"[F?Ø???]")   || //Black box
    _tcsstr(x,"[BlackBox]")	    ||
	_tcsstr(x,"BYPASS")	    ||
	_tcsstr(x,"ByPass")		||
	_tcsstr(x,"byPass")		||
	_tcsstr(x,"Bypass")		||
	_tcsstr(x,"CAPOTE")	    ||
	_tcsstr(x,"Capote")	    ||
	_tcsstr(x,"capote")	    ||	
	_tcsstr(x,"CATASTROPHE")	||
	_tcsstr(x,"Catastrophe")	||
	_tcsstr(x,"catastrophe")	||
	_tcsstr(x,"CHEAT")	    ||
	_tcsstr(x,"Cheat")	    ||
	_tcsstr(x,"cheat")	    ||
	_tcsstr(x,"CHEATER")	||
	_tcsstr(x,"Cheater")	||
	_tcsstr(x,"cheater")	||
	_tcsstr(x,"CHEATS")     ||
	_tcsstr(x,"Cheats")     ||
	_tcsstr(x,"cheats")     ||
	_tcsstr(x,"DUPAR")		||
	_tcsstr(x,"Dupar")		||
	_tcsstr(x,"dupar")		||
	_tcsstr(x,"DUPER")		||
	_tcsstr(x,"Duper")		||
	_tcsstr(x,"duper")		||
	_tcsstr(x,"ENGINE")	    ||
	//_tcsstr(x,"Engine")	    ||
	_tcsstr(x,"engine")	    ||
	_tcsstr(x,"EXPLORER")	||
	//_tcsstr(x,"Explorer")	||
	_tcsstr(x,"explorer")	||
	_tcsstr(x,"PROCESS")	||
	//_tcsstr(x,"Process")	||
	_tcsstr(x,"process")    ||
	_tcsstr(x,"FREEZE")		||
	_tcsstr(x,"Freeze")		||
	_tcsstr(x,"freeze")		||
	_tcsstr(x,"SPEED")      ||
	_tcsstr(x,"Speed")      ||
	_tcsstr(x,"Speed")      ||
	_tcsstr(x,"HACK")		||
	_tcsstr(x,"Hack")		||
	_tcsstr(x,"hack")		||
	_tcsstr(x,"HACKER")		||
	_tcsstr(x,"Hacker")		||
	_tcsstr(x,"hacker")		||
	_tcsstr(x,"HIDE")		||
	_tcsstr(x,"Hide")		||
	_tcsstr(x,"hide")		||
	_tcsstr(x,"TOOLZ")		||
	_tcsstr(x,"Toolz")		||
	_tcsstr(x,"toolz")		||
	_tcsstr(x,"INJECT")	    ||
	_tcsstr(x,"Inject")	    ||
	_tcsstr(x,"inject")	    ||
	_tcsstr(x,"INJECTOR")	||
	_tcsstr(x,"Injector")	||
	_tcsstr(x,"injector")	||
	_tcsstr(x,"KILL")	    ||
	_tcsstr(x,"Kill")		||
	_tcsstr(x,"kill")		||
	_tcsstr(x,"MUPIE")		||
	_tcsstr(x,"MuPie")		||
	_tcsstr(x,"MuPie")		||
	_tcsstr(x,"OLLY")		||
	_tcsstr(x,"Olly")		||
	_tcsstr(x,"olly")		||
	_tcsstr(x,"PACKAGER")	||
	_tcsstr(x,"Packager")	||
	_tcsstr(x,"Packager")	||
	_tcsstr(x,"PACOTES")	||
	_tcsstr(x,"Pacotes")	||
	_tcsstr(x,"Pacotes")	||
	_tcsstr(x,"SUSPEND")	||
	_tcsstr(x,"Suspend")	||
	_tcsstr(x,"suspend")	||
	_tcsstr(x,"WILDPROXY")	||
	_tcsstr(x,"Wildproxy")	||
	_tcsstr(x,"wildproxy")	||
	_tcsstr(x,"WPE")    	||
	_tcsstr(x,"Wpe")	    ||
	_tcsstr(x,"wpe")	    ||
	_tcsstr(x,"XELERATOR")	||
	_tcsstr(x,"Xelerator")	||
	_tcsstr(x,"xelerator")	||	
	_tcsstr(x,"XTRAP")	    ||
	_tcsstr(x,"XTrap")	    ||
	_tcsstr(x,"xtrap")	    ||
	_tcsstr(x,"XSPEED")	    ||
	_tcsstr(x,"XSpeed")	    ||
	_tcsstr(x,"xSpeed")	    ||
	_tcsstr(x,"xspeed")	    ||	
	_tcsstr(x,"FUNNYZHYPER")	||
	_tcsstr(x,"FunnyZhyper")	||
	_tcsstr(x,"funnyzhyper")	||
    _tcsstr(x,"AUTOSKILL")	||
	_tcsstr(x,"AutoSkill")	||
	_tcsstr(x,"autoskill")	||
	_tcsstr(x,"AUTOKILL")	||
	_tcsstr(x,"Autokill")	||
	_tcsstr(x,"autokill")	||
	_tcsstr(x,"AGILITY")	||
	_tcsstr(x,"Agility")	||
	_tcsstr(x,"agility")	||
	_tcsstr(x,"TRADE")	    ||
	_tcsstr(x,"Trade")	    ||
	_tcsstr(x,"trade")	    ||
	_tcsstr(x,"PACKET")	    ||
	_tcsstr(x,"Packet")	    ||
	_tcsstr(x,"packet")	    ||
	_tcsstr(x,"AUTOIT")	    ||
	_tcsstr(x,"AutoIt")	    ||
	_tcsstr(x,"autoit")	    ||
	_tcsstr(x,"AUTOKEYBOARD") ||
	_tcsstr(x,"AutoKeyboard") ||
	_tcsstr(x,"autokeyboard") ||	
	_tcsstr(x,"SANDBOX")     ||
	_tcsstr(x,"Sandbox")     ||
	_tcsstr(x,"sandbox")     ||
	_tcsstr(x,"SANDBOXED")   ||
	_tcsstr(x,"Sandboxed")   ||
	_tcsstr(x,"sandboxed")   ||
	_tcsstr(x,"DEFALTBOX")   ||
	_tcsstr(x,"DefaltBox")   ||
	_tcsstr(x,"defaltbox")   ||
	_tcsstr(x,"HXD")         ||
	_tcsstr(x,"HxD")         ||
	_tcsstr(x,"hxd")         ||
	_tcsstr(x,"BVKHEX")      ||
	_tcsstr(x,"Bvkhex")      ||
	_tcsstr(x,"bvkhex")      ||
	_tcsstr(x,"bvkhex")      ||
	_tcsstr(x,"bvkhex")      ||
	_tcsstr(x,"Emulator:")   ||   //SolidProxy Stealth mode
	_tcsstr(x,"AutoClicker") ||
	_tcsstr(x,"Autoclicker") ||
	_tcsstr(x,"autoclicker") ||
	_tcsstr(x,"(Elite)")     ||
	_tcsstr(x,"Elite")       ||
	_tcsstr(x,"Style30_Theme1")  || //darkterro gamehacks
	_tcsstr(x,"DEBUG")       ||
	_tcsstr(x,"DEBUG")       ||
	_tcsstr(x,"Debug")       ||
	_tcsstr(x,"debug")){


	LogsAdded ("[Closed]: %s" , x);

	ExitProcess(0);				
    return FALSE;
    }
	//cout <<"true";
	return TRUE;
}


void heuristica(){
	EnumWindows(heurprocess, NULL);
}

void heuristico()
{

again:
 
    heuristica();
    Sleep (10000);
    goto again;

}

void HeuristicW()
{
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(heuristico),NULL,0,0);
}
//=======================================================================================

BOOL CALLBACK heurclass(HWND hwnd, LPARAM lParam){
    static TCHAR y[60];
    GetClassName(hwnd, y, 60);

////////////////////////////////////////////////////////////////////////////////////////////////
//Heuristic ClassWindow - HC-Scan 
//Are NON Case-sensitive - Find it using [Handler 1.5 by Oliver Bock / Classname] 
//_tcsstr(y,"xxxx")  Just one word, without empty space
////////////////////////////////////////////////////////////////////////////////////////////////
if (_tcsstr(y,"ProcessHacker")          || 
	_tcsstr(y,"PROCEXP")                || //ProcessExplorer
	_tcsstr(y,"TFormMain.UnicodeClass") || //HxD
	//_tcsstr(y,"WindowsForms10")         || 	 
	_tcsstr(y,"obj_Form")){                //Hidetoolz


	LogsAdded ("[Closed]: %s" , y);
	ExitProcess(0);				
    return FALSE;
    }
	//cout <<"true";
	return TRUE;
}


void heuristiclass(){
	EnumWindows(heurclass, NULL);
}

void heuri11()
{
again:
    heuristiclass();
    Sleep (10000);
    goto again;

}

void Heuristicclass()
{
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(heuri11),NULL,0,0);
}