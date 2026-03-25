#pragma once
#define MAIN_RESOLUTION			0x05877D58
#define MAIN_RESOLUTION_X		0x007E7F68
#define MAIN_RESOLUTION_Y		0x007E7F6C
#define MAIN_FONT_SIZE			0x078C5260

#define CASH_RESOLUTION_X       0x0072F787
#define CASH_RESOLUTION_Y       0x0072F791
void InitResolution();
void ResolutionSwitch();
void ResolutionSwitchFont();
void ResolutionMoveList();
void ResolutionMoveList2();
void ResolutionKanturu();
//
//
//#define m_Resolution *(int*)0x05877D58
//
//
//void InitResolution();
//
//
//void ResolutionSwitch();
//
//
//void ResolutionSwitchFont();
//
//
//void ResolutionMoveList();
//
//
//void ResolutionMoveList2();
//
//
//bool ResolutionKanturu();