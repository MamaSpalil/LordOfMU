#include "stdafx.h"
#include "InterfaceBase.h"
#include "Interface.h"
#include "Structure.h"
#include "Other.h"
//
//cInterfaceBase::cInterfaceBase()
//{
//	SetWindow(false);
//}
//
//cInterfaceBase::~cInterfaceBase()
//{
//
//}
//
//void cInterfaceBase::ToggleWindow(bool mode)
//{
//	SetWindow(!CheckWindow(), mode);
//}
//void cInterfaceBase::OpenWindow(bool mode)
//{
//	SetWindow(true, mode);
//}
//void cInterfaceBase::CloseWindow(bool mode)
//{
//	SetWindow(false, mode);
//}
//void cInterfaceBase::SetWindow(bool state, bool mode)
//{
//	this->Drawing = state;
//	if (mode)
//		CheckWindow() ? OnOpen() : OnClose();
//}
//bool cInterfaceBase::CheckWindow()
//{
//	return this->Drawing;
//}
//
//void cInterfaceBase::OnOpen()
//{
//
//}
//void cInterfaceBase::OnClose()
//{
//
//}
//
//void cInterfaceBase::SetRect(float a1, float a2, float a3, float a4)
//{
//	this->x = a1;
//	this->y = a2;
//	this->w = a3;
//	this->h = a4;
//}
//
//bool cInterfaceBase::IsOnWindow()
//{
//	return *(HWND*)0x07AFEE84 == GetForegroundWindow();
//}
//
//void cInterfaceBase::DrawButton(float x, float y, float w, float h, PUSHEVENT_CALLBACK_LPVOID pCallbackFunc)
//{
//    if (CheckMouseIn(x, y, w, h))
//    {
//        if (GetAsyncKeyState(VK_LBUTTON))
//        {
//            pCallbackFunc(this);
//            return;
//        }
//
//        return;
//    }
//
//    return;
//}
