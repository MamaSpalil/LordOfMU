#include "stdafx.h"
#include "Interface.h"
#include "InterfaceBase.h"
#include "Interface.h"
#include "DailyReward.h"
#include "Other.h"
#include "Structure.h"
#include <ctime>
//
//cDailyReward DailyReward;
//
//cDailyReward::cDailyReward()
//{
//    this->Init();
//
//    this->MainWidth = 440.0f;
//
//    this->MainHeight = 270.0f;
//
//    this->MainPosX = ImgCenterScreenPosX(this->MainWidth);
//
//    this->MainPosY = ImgCenterScreenPosY(this->MainHeight);
//
//    this->SectionWidth = 100;
//
//    this->SectionPosY = this->MainPosY + 20.0f;
//
//    this->GeneralPosX = 240.0f;
//
//}
//
//cDailyReward::~cDailyReward()
//{
//
//}
//
//void cDailyReward::Init()
//{
//    this->OpenDaily = false;
//    this->SetWindow(false);
//    this->SetRect(120, 70, 400, 255);
//    this->SelectedDay = 1;
//}
//
//unsigned char last_day_of_month(int year, unsigned char month) {
//    return month != 2 ? ((month ^ (month >> 3))) | 30 :
//        year % 4 ? 29 : 28;
//}
//
//void cDailyReward::ToggleWindow(bool mode)
//{
//    SetWindow(!CheckWindow(), mode);
//}
//void cDailyReward::OpenWindow(bool mode)
//{
//    SetWindow(true, mode);
//}
//void cDailyReward::CloseWindow(bool mode)
//{
//    SetWindow(false, mode);
//}
//
//void cDailyReward::SetWindow(bool state, bool mode)
//{
//    this->Drawing = state;
//    if (mode)
//        CheckWindow() ? OnOpen() : OnClose();
//}
//
//void cDailyReward::OnOpen()
//{
//    this->Loaded = false;
//    this->DayInfo = false;
//    this->SendRequest(0x0, 0x0);
//}
//void cDailyReward::OnClose()
//{
//
//}
//
//void cDailyReward::ClearDayGift()
//{
//    this->DayInfo   = false;
//    this->Type      = -1;
//    this->Level     = 0;
//    this->Ex        = 0;
//    this->Zen       = 0;
//    this->Wcoin     = 0;
//}
//
//int ImgTag100 = 0;
//
//void cDailyReward::DrawIcon()
//{
//    if (!Blink)
//    {
//        return;
//    }
//
//    EnableAlphaTest(1);
//    glColor3f(1.0, 1.0, 1.0);
//    glColor4f(1.0, 1.0, 1.0, 1.0);
//
//
//    if(this->OpenDaily == false)
//    { 
//		RenderBitmap(178, 10.0, 160.0, 90.0, 90.0, 0.f, 0.f, 0.95, 0.95, 0, 1, 0.0);
//    }
//    else
//    {
//        RenderBitmap(179, 10.0, 160.0, 90.0, 90.0, 0.f, 0.f, 0.95, 0.95, 0, 1, 0.0);
//
//    }
//
//    if (gFullMap.OpenFullMap == true)
//    {
//        return;
//    }
//
//
//    if (pCheckWindow(WINDOWS_MOVE_LIST) || pCheckWindow(WINDOWS_CHARACTER) || pCheckWindow(WINDOWS_INVENTORY) || pCheckWindow(WINDOWS_QUEST) || pCheckWindow(WINDOWS_PET))
//    {
//        this->OpenDaily = false;
//    }
//
//    if (CheckMouseIn(10, 160, 30, 30))
//    {
//        this->DrawMain();
//
//        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && IsOnWindow())
//        {
//            this->OpenDaily = !this->OpenDaily;
//
//            if (!this->OpenDaily)
//            {
//                SetCursorFocus(false);
//            }
//            else
//            {
//                SetCursorFocus(true);
//            }
//        }
//    }
//
//    glEnable(GL_TEXTURE_2D);
//    EnableAlphaBlend();
//}
//
//void cDailyReward::Draw()
//{
//     this->DrawIcon();
//
//     if (this->OpenDaily == true)
//     {
//         this->DrawBase();
//     }
//}
//
//void cDailyReward::DrawBase()
//{
//    if (pCheckWindow(WINDOWS_INVENTORY) || pCheckWindow(WINDOWS_SHOP) || pCheckWindow(WINDOWS_STORE_OTHER) || pCheckWindow(WINDOWS_CHARACTER) || pCheckWindow(WINDOWS_STORE) || pCheckWindow(WINDOWS_WAREHOUSE) || GetKeyState(VK_ESCAPE) & 0x8000)
//    {
//        keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
//        SetCursorFocus(false);
//        this->OpenDaily = false;
//        return;
//    }
//
//    EnableAlphaTest(1);
//
//    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
//
//    pDrawBarForm2(this->MainPosX, this->MainPosY, this->MainWidth, this->MainHeight);
//
//    glColor3f(1.0, 1.0, 1.0);
//
//    DisableAlphaBlend();
//
//    pDrawLegend(0, 0, "");
//    pSetBackgroundTextColor(pTextThis2(), 153, 0, 0, 255);
//    pDrawText2(pTextThis2(), (int)this->MainPosX + 5, (int)this->MainPosY + 5, "REWARD SYSTEM", (int)this->MainWidth - 10, 0, 3, 0);
//    pDrawLegend(0, 0, "");
//
//    tm today;
//    time_t ltime;
//    time(&ltime);
//    localtime_s(&today, &ltime);
//
//    char* DoW[] = { "DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB" };
//
//    float sW = 35, sH = 38;
//    int i, v1, v2, v3, v4, v5;
//    v3 = this->FWday;
//    v4 = this->MaxDay;
//
//    for (i = 0; i < 7; i++)
//    {      
//        pDrawLegend(0, 0, "");
//        pBackgroundTextColor(pTextThis(), 125, 125,125, 125);
//        pSetTextColor(pTextThis(), 255, 255, 255, 180);
//        pDrawTextOut(pTextThis(), x + 15 + sW * i, y + 42, DoW[i], sW - 2, 0, (LPINT)3, 0);
//        pDrawLegend(0, 0, "");
//
//    }
//    
//    if (Loaded)
//    {
//        for (i = v3; i < (v3 + v4); i++)
//        {
//            v1 = i % cDR_SPLIT_DAY;
//            v2 = (i - v1) / cDR_SPLIT_DAY;
//            v5 = i - v3 + 1;
//
//            EnableAlphaTest(1);
//
//            glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
//
//            pDrawBarForm2(x + 9 + sW * v1 + (sW - 16) / 2, y + 77 + v2 * sH, 27, 10);
//
//            glColor3f(1.0, 1.0, 1.0);
//
//            DisableAlphaBlend();
//
//
//            if (SelectedDay == v5)
//            {
//                EnableAlphaTest(1);
//
//                glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
//
//                pDrawBarForm2(x + 19 + sW * v1, y + 77 + v2 * sH, 26, 10);
//
//                glColor3f(1.0, 1.0, 1.0);
//
//                DisableAlphaBlend();
//            }
//
//
//            EnableAlphaTest(1);
//            glColor3f(1.0, 1.0, 1.0);
//            glColor4f(1.0, 1.0, 1.0, 1.0);
//
//            if (SelectedDay == v5 && State[v5 - 1] == 0)
//            {
//                RenderBitmap(178, x + 7 + sW * v1 + (sW - 16) / 2, y + 48 + v2 * sH, 45, 45, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
//            }
//
//            else if (CheckMouseIn(x + 15 + sW * v1, y + 52 + v2 * sH, sW - 2, sH - 2))
//            {
//                RenderBitmap(178, x + 7 + sW * v1 + (sW - 16) / 2, y + 48 + v2 * sH, 45, 45, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
//               
//                if (GetAsyncKeyState(VK_LBUTTON))
//                {
//                    SelectedDay = v5;
//
//                    if (State[v5 - 1] == 0)
//                    {
//                        this->SendRequest(0x01, SelectedDay);
//                        this->ClearDayGift();
//                    }
//                }
//            }
//            else if (State[v5 - 1] == 0) //
//            {
//                RenderBitmap(178, x + 7 + sW * v1 + (sW - 16) / 2, y + 48 + v2 * sH, 45, 45, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
//            }
//            else
//            {
//                RenderBitmap(179, x + 7 + sW * v1 + (sW - 16) / 2, y + 48 + v2 * sH, 45, 45, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
//            }
//
//            char infor[256];
//            sprintf_s(infor, "%d", v5);
//
//            pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//            pSetTextColor(pTextThis(), 255, 255, 255, 180);
//            pDrawTextOut(pTextThis(), x + 15 + sW * v1, y + 52 + v2 * sH + sH - 13, infor, sW - 2, 2, (LPINT)3, 0);
//
//            glEnable(GL_TEXTURE_2D);
//            EnableAlphaBlend();
//        }
//
//        if (SelectedDay != 0)
//        {
//            if (State[SelectedDay - 1] != 0)
//            {
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 265, y + 100, "CONCLUIDO", 125, 0, (LPINT)3, 0);
//
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 265, y + 110, "Esta recompensa foi resgatada,", 125, 0, (LPINT)3, 0);
//
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 265, y + 120, "continue jogando todos os dias", 125, 0, (LPINT)3, 0);
//
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 265, y + 130, "para resgatar mais recompensas!", 125, 0, (LPINT)3, 0);
//
//            }
//            else if (DayInfo)
//            {
//                char info[2][256];
//                sprintf_s(info[0], "PCPoint: %d", Wcoin);
//                sprintf_s(info[1], "Zen : %d", Zen);
//
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 260, y + 50, Text, 129, 0, (LPINT)3, 0);
//                pDrawTextOut(pTextThis(), x + 260, y + 62, info[0], 129, 0, (LPINT)3, 0);
//                pDrawTextOut(pTextThis(), x + 260, y + 74, info[1], 129, 0, (LPINT)3, 0);
//
//
//                if (SelectedDay < today.tm_mday)
//                {
//
//                    EnableAlphaTest(1);
//
//                    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
//
//                    if (CheckMouseIn(x - 5 + 74 * 4, y + h - 50, 73, 27))
//                    {
//                        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
//
//                    }
//
//                    pDrawBarForm2(x - 5 + 74 * 4, y + h - 50, 73, 27);
//
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//
//                    this->DrawButton(x - 5 + 74 * 4, y + h - 50, 73, 27, DailyReward.RecvGiftAction);
//
//                    EnableAlphaTest(1);
//                    glColor4f(1.0, 1.0, 1.0, 1.0);
//                    pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                    pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                    pDrawTextOut(pTextThis(), x - 5 + 74 * 4, y + h - 42.5, "Indisponivel", 73, 0, (LPINT)3, 0);
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//
//                }
//
//                else if (SelectedDay == today.tm_mday)
//                {
//                    EnableAlphaTest(1);
//
//                    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
//
//                    if (CheckMouseIn(x - 5 + 74 * 4, y + h - 50, 73, 27))
//                    {
//                        glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
//                    }
//
//                    pDrawBarForm2(x - 5 + 74 * 4, y + h - 50, 73, 27);
//
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//
//                    this->DrawButton(x - 5 + 74 * 4, y + h - 50, 73, 27, DailyReward.RecvGiftAction);
//
//                    EnableAlphaTest(1);
//                    glColor4f(1.0, 1.0, 1.0, 1.0);
//                    pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                    pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                    pDrawTextOut(pTextThis(), x - 5 + 74 * 4, y + h - 42.5, "Resgatar", 73, 0, (LPINT)3, 0);
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//                }
//
//                else if (SelectedDay > today.tm_mday)
//                {
//                    EnableAlphaTest(1);
//
//                    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
//
//                    if (CheckMouseIn(x - 5 + 74 * 4, y + h - 50, 73, 27))
//                    {
//                        glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
//
//                    }
//                    pDrawBarForm2(x - 5 + 74 * 4, y + h - 50, 73, 27);
//
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//
//                    this->DrawButton(x - 5 + 74 * 4, y + h - 50, 73, 27,DailyReward.RecvGiftAction);
//
//                    EnableAlphaTest(1);
//                    glColor4f(1.0, 1.0, 1.0, 1.0);
//                    pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                    pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                    pDrawTextOut(pTextThis(), x - 5 + 74 * 4, y + h - 42.5, "Em Breve", 73, 0, (LPINT)3, 0);
//                    glColor3f(1.0, 1.0, 1.0);
//
//                    DisableAlphaBlend();
//
//                }
//                    
//
//                if (this->Type < 2960) // Armas
//                {
//                    DrawItemIMG(x + 280, y + 100, 70, 85, this->Type, this->Level, this->Ex, 0, 0);
//                }
//                else if (this->Type >= 6144 && this->Type < 6444) // Asas
//                {
//                    DrawItemIMG(x + 280, y + 100, 90, 90, this->Type, this->Level, this->Ex, 0, 0);
//                }
//                else
//                {
//                    DrawItemIMG(x + 280, y + 100, 90, 65, this->Type, this->Level, this->Ex, 0, 0);
//                }
//
//            }
//            else
//            {
//                pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//                pSetTextColor(pTextThis(), 255, 255, 255, 180);
//                pDrawTextOut(pTextThis(), x + 260, y + 50, "Sem informações...", 130, 0, (LPINT)3, 0);
//
//            }
//        }
//    }
//    else
//    {
//        pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//        pSetTextColor(pTextThis(), 255, 255, 255, 180);
//        pDrawTextOut(pTextThis(), x + 60, y + 50, "Sem informações...", 200, 0, (LPINT)3, 0);
//    }
//}
//
//void cDailyReward::DrawMain()
//{
//
//    if (this->OpenDaily == true)
//    {
//        pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//        pSetTextColor(pTextThis(), 255, 255, 255, 180);
//        pDrawTextOut(pTextThis(), (int)10, (int)195, "Press To Close", 0, 0, (LPINT)1, 0);
//
//        return;
//    }
//
//    EnableAlphaTest(1);
//
//    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
//
//    pDrawBarForm2(65, 155, 100, 30);
//
//    glColor3f(1.0, 1.0, 1.0);
//
//    DisableAlphaBlend();
//
//    EnableAlphaTest(1);
//    glColor4f(1.0, 1.0, 1.0, 1.0);
//
//    pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//    pSetTextColor(pTextThis(), 255, 0, 0, 180);
//    pDrawTextOut(pTextThis(), (int)70, (int)160, "- Reward Daily: ", 0, 0, (LPINT)1, 0);
//
//    pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
//    pSetTextColor(pTextThis(), 255, 255, 255, 180);
//    pDrawTextOut(pTextThis(), (int)73, (int)170, "Check Date to Present Box", 0, 0, (LPINT)1, 0);
//    
//
//    glColor3f(1.0, 1.0, 1.0);
//
//    DisableAlphaBlend();
//
//}
//
//void cDailyReward::SendRequest(BYTE type, int value)
//{
//    PMSG_DAILY_REWARD_SEND pRequest;
//    
//    pRequest.h.set((LPBYTE)&pRequest, 0xFB, 0x02, sizeof(pRequest));
//    pRequest.Type = type;
//    pRequest.Value = value;
//
//    DataSend((LPBYTE)&pRequest, pRequest.h.size);
//}
//
//bool cDailyReward::AllowedGift()
//{
//    return Loaded && State[SelectedDay - 1] == 0 && SelectedDay <= CurDay;
//}
//
//void cDailyReward::RecvGiftAction(LPVOID pClass)
//{
//    cDailyReward* This = (cDailyReward*)pClass;
//
//    if (This->AllowedGift())
//    {
//        This->SendRequest(0x02, This->SelectedDay);
//    }
//}
//
//void cDailyReward::GCRecvUserInfo(BYTE* a)
//{
//    PMSG_DAILY_REWARD_USER_INFO_RECV* Data = (PMSG_DAILY_REWARD_USER_INFO_RECV*)a;
//    this->CurDay = Data->CurDay;
//    this->MaxDay = Data->MaxDay;
//    this->FWday = Data->FWday;
//    this->Timer = Data->Timer;
//
//    for (int i = 0; i < 31; i++)
//    {
//        this->State[i] = Data->State[i];
//    }
//
//    if (State[CurDay - 1] == 0)
//    {
//        Blink = true;
//    }
//    else
//    {
//        Blink = false;
//    }
//
//    Loaded = true;
//}
//
//int cDailyReward::CheckItemInfo(signed int a1)
//{
//    if (a1 > 3092)
//    {
//        if (a1 > 4661)
//        {
//            if (a1 > 5685 || a1 < 5676 && (a1 < 5164 || a1 > 5173))
//                return 0;
//        }
//        else if (a1 < 4652 && (a1 > 4149 || a1 < 4140 && (a1 < 3628 || a1 > 3637)))
//        {
//            return 0;
//        }
//
//        return 1;
//    }
//
//    if (a1 >= 3089)
//        return 1;
//
//    if (a1 > 2071)
//    {
//        if (a1 != 2580 && (a1 <= 2589 || a1 > 2592))
//            return 0;
//        return 1;
//    }
//
//    if (a1 == 2071 || a1 <= 1041 && (a1 >= 1040 || a1 >= 26 && a1 <= 28))
//        return 1;
//
//    return 0;
//}
//
//void cDailyReward::GCRecvDayInfo(BYTE* a)
//{
//    PMSG_DAILY_REWARD_DAY_INFO_RECV* Data = (PMSG_DAILY_REWARD_DAY_INFO_RECV*)a;
//
//    DayInfo = true;
//
//    if (Data->Type != -1)
//    {
//        this->Type = Data->Type;
//
//        DWORD dwAddress;
//        int a5;
//        int a6;
//        int a7;
//        int a8;
//        int a9;
//        DWORD a10;
//
//        dwAddress = (DWORD)&this->Item;
//
//        *(WORD*)dwAddress = Data->Type;
//        a5 = Data->Dur;
//        *(BYTE*)(dwAddress + 22) = a5;
//        a6 = Data->Item[3];
//        *(BYTE*)(dwAddress + 23) = a6;
//        a7 = Data->Item[4];
//        *(BYTE*)(dwAddress + 24) = a7;
//        a8 = Data->Item[5];
//        *(BYTE*)(dwAddress + 97) = (a8 & 2) >> 1 > 0;
//        *(BYTE*)(dwAddress + 98) = (a8 & 4) >> 2 > 0;
//        *(BYTE*)(dwAddress + 73) = 16 * (a8 & 8) >> 7 > 0;
//        a9 = Data->Item[6];
//        *(WORD*)(dwAddress + 69) = (a9 & 0xF0) >> 4;
//        *(WORD*)(dwAddress + 71) = a9 & 0xF;
//
//        a10 = (DWORD)&Data->Item[7];
//
//        if (a10)
//        {
//            *(BYTE*)(dwAddress + 79) = 5;
//
//            for (int i = 0; i < 5; ++i)
//                *(BYTE*)(i + dwAddress + 74) = *(BYTE*)(i + a10);
//
//            for (int j = 0; j < 5; ++j)
//            {
//                if (*(BYTE*)(j + a10) == 255)
//                {
//                    *(BYTE*)(dwAddress + 79) = j;
//                    break;
//                }
//
//                if (*(BYTE*)(j + a10) == 254)
//                {
//                    *(BYTE*)(j + dwAddress + 80) = -1;
//                }
//                else
//                {
//                    *(BYTE*)(j + dwAddress + 80) = *(BYTE*)(j + a10) % 50;
//                    *(BYTE*)(j + dwAddress + 85) = *(BYTE*)(j + a10) / 50 + 1;
//                }
//            }
//
//            if (this->CheckItemInfo(*(WORD*)dwAddress))
//            {
//                *(BYTE*)(dwAddress + 90) = a9;
//                *(WORD*)(dwAddress + 69) = 0;
//                *(WORD*)(dwAddress + 71) = 0;
//            }
//            else
//            {
//                *(BYTE*)(dwAddress + 90) = -1;
//            }
//        }
//        else
//        {
//            *(BYTE*)(dwAddress + 79) = 0;
//        }
//
//        *(BYTE*)(dwAddress + 96) = 0;
//        *(DWORD*)(dwAddress + 103) = 1;
//
//        if (Data->PeriodTime)
//        {
//            this->Item.PeriodItem = 1;
//            this->Item.ExpireDateConvert = Data->PeriodTime;
//        }
//
//       // ((void(__cdecl*)(ObjectItem * lpItem, BYTE Option, BYTE Special, BYTE Value)) 0x58B910)(&this->Item, Data->Item[1], Data->Item[3], Data->Item[4]);
//    }
//    this->Zen = Data->Zen;
//    this->Wcoin = Data->Wcoin;
//    memcpy_s(Text, 100, Data->Text, 100);
//}