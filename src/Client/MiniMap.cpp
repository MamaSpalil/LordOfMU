#include "stdafx.h"
#include "MiniMap.h"
#include "Structure.h"
#include "TMemory.h"

#include "stdafx.h"
#include "MiniMap.h"
#include "TMemory.h"



bool MiniMapTable[MAX_MINI_MAP];

void InitMiniMap() // OK
{
    memset(MiniMapTable,0,sizeof(MiniMapTable)); // 1.04J DONE

    SetCompleteHook(0xE8,0x006AA5CC,&MiniMapLoad); // 1.04J DONE //6AA5CC
	SetCompleteHook(0xFF,0x006AE204,&MiniMapCore); // 1.04J DONE//6AE204
    SetCompleteHook(0xFF,0x0041315E,&MiniMapCheck); // 1.04J DONE//41315E
}


void MiniMapCore() // OK
{
    ((void(*)())0x006A926F)(); // 1.04J DONE//6A926F


    MiniMapLoad();
}


void MiniMapLoad() // OK
{
    if(*(int*)(MAIN_CURRENT_MAP) < MAX_MINI_MAP)
    {
        char buff[64];

        wsprintf(buff,"Customs\\MiniMaps\\MiniMap%d.jpg",(*(int*)(MAIN_CURRENT_MAP)+1));

        if(MiniMapFileCheck(*(int*)(MAIN_CURRENT_MAP)) != 0)
        {
            MiniMapTable[*(int*)(MAIN_CURRENT_MAP)] = 1;

			((bool(*)(char*,DWORD,DWORD,DWORD,BYTE))0x006D5B1B)(buff,2200,9729,10496,1);//6D5B1B

        }
    }
}


bool MiniMapCheck(int map) // OK
{
    if(map == 30 || (map < MAX_MINI_MAP && MiniMapTable[map] != 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


bool MiniMapFileCheck(int map) // OK
{
    if(map == 30){return 1;}


    char buff[64];

	
    wsprintf(buff,".\\Data\\Customs\\MiniMaps\\MiniMap%d.ozj",(map+1));


    FILE* file;


    if(fopen_s(&file,buff,"r") != 0)
    {
        return 0;
    }
    else
    {
        fclose(file);
        return 1;
    }
	
}
