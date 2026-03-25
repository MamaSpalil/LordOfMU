#include "stdafx.h"
#include "Other.h"
#include "QuickAccess.h"
#include "TMemory.h"
#include "define.h"

BITMAP_t* CGlobalBitmap(int This, GLuint uiImage)
{
	return((BITMAP_t*)(This + 56 * uiImage));
}

void RenderImageF(GLuint uiImage, float x, float y, float width, float height, float su, float sv, float uw, float vh)
{
	BITMAP_t* pText = CGlobalBitmap((int)&*(LPVOID*)0x7EB7278, uiImage);//07EB7278//07EB7278

	if (pText->Buffer)
	{
		RenderBitmap(uiImage, x, y, width, height, su / pText->Width, sv / pText->Height, uw / pText->Width, vh / pText->Height, 1, 1, 0.0);
	}
}

bool CheckMouseIn(int PosX, int PosY, int Width, int Height)
{
	return pIsWorkZone(PosX, PosY, Width, Height, 1);
}

CLoginWin gCLoginWin;

bool OnSystem = GetPrivateProfileIntA("MultiLogin","OnSystem",0,"./xAccounts.ini");

float LoginX;
float LoginY;

void CLoginWin::cSetPosition(BYTE *This, int nXCoord, int nYCoord, int eChangedPram)
{
	LoginX = nXCoord;
	LoginY = nYCoord;
	Sprite_SetPosition(This,nXCoord,nYCoord,eChangedPram);
}

char iUser1[32],iPassword1[32],iUser2[32],iPassword2[32],iUser3[32],iPassword3[32],iUser4[32],iPassword4[32],iUser5[32],iPassword5[32];
char iUser6[32],iPassword6[32],iUser7[32],iPassword7[32],iUser8[32],iPassword8[32],iUser9[32],iPassword9[32],iUser10[32],iPassword10[32];

void CButtonAutoLogin(int This, int a2)
{
	if(OnSystem == 1)
	{
		pSetBlend(1);
		glColor4f(1.0, 1.0, 1.0, 1.0);

		float PosXMove;
		float PosYMove = TransFormY(LoginY) + TransFormY(62);
		float PosYMoveB = TransFormY(LoginY) + TransFormY(70);

		//MOV EAX,DWORD PTR DS:[5877AE8]
		//CUIRenderText_SetFont((int)pTextThis(),*(int*)0x7AFEEA0);//MOV EBP,DWORD PTR DS:[77A8FDC]
		//[5877FAC]
		int Struct = *(DWORD*)0x5877F40;//5877F40//38

		GetPrivateProfileString("MultiLogin","User1","",iUser1,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password1","",iPassword1,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User2","",iUser2,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password2","",iPassword2,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User3","",iUser3,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password3","",iPassword3,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User4","",iUser4,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password4","",iPassword4,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User5","",iUser5,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password5","",iPassword5,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User6","",iUser6,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password6","",iPassword6,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User7","",iUser7,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password7","",iPassword7,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User8","",iUser8,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password8","",iPassword8,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User9","",iUser9,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password9","",iPassword9,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","User10","",iUser10,32,".\\xAccounts.ini");
		GetPrivateProfileString("MultiLogin","Password10","",iPassword10,32,".\\xAccounts.ini");

		int Amount = GetPrivateProfileIntA("MultiLogin","Amount",0,"./xAccounts.ini");

		if(Amount == 1){PosXMove = TransFormX(261);}
		else if(Amount == 2){PosXMove = TransFormX(232);}
		else if(Amount == 3){PosXMove = TransFormX(203);}
		else if(Amount == 4){PosXMove = TransFormX(174);}
		else if(Amount == 5){PosXMove = TransFormX(145);}
		else if(Amount == 6){PosXMove = TransFormX(116);}
		else if(Amount == 7){PosXMove = TransFormX(87);}
		else if(Amount == 8){PosXMove = TransFormX(58);}
		else if(Amount == 9){PosXMove = TransFormX(29);}
		else if(Amount == 10){PosXMove = TransFormX(0);}

		float X = TransFormX(LoginX)-TransFormX(368);

		//pBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
		//pSetTextColor(pTextThis(), 255, 255, 255, 255);
		//MU_DrawTextOut( X + PosXMove + TransFormX(130), PosYMoveB-18, "Quick Access:", 100, 0, 0);

		if(Amount > 0)
		{
			float v22 = 18.0 / 32.0;
			float v23 = 30.0 / 32.0;
			//pDrawImage2(240, X + PosXMove + TransFormX(12), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			//pDrawImage2(3695,X + PosXMove + TransFormX(22), PosYMove,50,25,0.0,0.0,0.8128125, 0.8228125, 1, 1, 0.0);//0.6875 / 0.8828125
			RenderImageF(151, X + PosXMove + TransFormX(32), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
			//RenderBitmap(151, X + PosXMove + TransFormX(34), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
			MU_DrawTextOut( X + PosXMove + TransFormX(36), PosYMoveB, iUser1, TransFormX(65), 3, 0);
			//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(32), PosYMoveB, iUser1, TransFormX(57), 0, (LPINT)3, 0);
			if(CheckMouseIn(X + PosXMove + TransFormX(32), PosYMove, TransFormX(57), TransFormY(24)))
			{
				if (LeftClick > 0)
				{
					pPlayBuffer(25, 0, 0);
					InputBox_SetText((Struct + 4), iUser1);
					InputBox_SetText((Struct + 240), iPassword1);
					RequestLogin(Struct);
					LeftClick = 0;
				}
			}
			if(Amount > 1)
			{
				//pDrawImage2(240, X + PosXMove + TransFormX(90), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
				RenderImageF(151, X + PosXMove + TransFormX(90), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
				//RenderBitmap(223, X + PosXMove + TransFormX(92), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
				MU_DrawTextOut( X + PosXMove + TransFormX(96), PosYMoveB, iUser2, TransFormX(65), 3, 0);
				//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(90), PosYMoveB, iUser2, TransFormX(57), 0, (LPINT)3, 0);
				if(CheckMouseIn(X + PosXMove + TransFormX(90), PosYMove, TransFormX(57), TransFormY(24)))
				{
					if (LeftClick > 0)
					{
						pPlayBuffer(25, 0, 0);
						InputBox_SetText((Struct + 4), iUser2);
						InputBox_SetText((Struct + 240), iPassword2);
						RequestLogin(Struct);
					}
				}
				//****************
				if(Amount > 2)
				{
					//pDrawImage2(240, X + PosXMove + TransFormX(168), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
					RenderImageF(151, X + PosXMove + TransFormX(148), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
					//RenderBitmap(223, X + PosXMove + TransFormX(150), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
					MU_DrawTextOut( X + PosXMove + TransFormX(152), PosYMoveB, iUser3, TransFormX(65), 3, 0);
					//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(148), PosYMoveB, iUser3, TransFormX(57), 0, (LPINT)3, 0);
					if(CheckMouseIn(X + PosXMove + TransFormX(148), PosYMove, TransFormX(57), TransFormY(24)))
					{
						if (LeftClick > 0)
						{
							pPlayBuffer(25, 0, 0);
							InputBox_SetText((Struct + 4), iUser3);
							InputBox_SetText((Struct + 240), iPassword3);
							RequestLogin(Struct);
						}
					}
					//****************
					if(Amount > 3)
					{
						//pDrawImage2(240, X + PosXMove + TransFormX(246), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
						RenderImageF(151, X + PosXMove + TransFormX(206), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
						//RenderBitmap(223, X + PosXMove + TransFormX(208), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
						//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(206), PosYMoveB, iUser4, TransFormX(57), 0, (LPINT)3, 0);
						MU_DrawTextOut( X + PosXMove + TransFormX(210), PosYMoveB, iUser4, TransFormX(65), 3, 0);
						if(CheckMouseIn(X + PosXMove + TransFormX(206), PosYMove, TransFormX(57), TransFormY(24)))
						{
							if (LeftClick > 0)
							{
								pPlayBuffer(25, 0, 0);
								InputBox_SetText((Struct + 4), iUser4);
								InputBox_SetText((Struct + 240), iPassword4);
								RequestLogin(Struct);
							}
						}
						//****************
						if(Amount > 4)
						{
							//pDrawImage2(240, X + PosXMove + TransFormX(324), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
							//RenderBitmap(223, X + PosXMove + TransFormX(266), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
							RenderImageF(151, X + PosXMove + TransFormX(264), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
							//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(264), PosYMoveB, iUser5, TransFormX(57), 0, (LPINT)3, 0);
							MU_DrawTextOut( X + PosXMove + TransFormX(268), PosYMoveB, iUser5, TransFormX(65), 3, 0);
							if(CheckMouseIn(X + PosXMove + TransFormX(264), PosYMove, TransFormX(57), TransFormY(24)))
							{
								if (LeftClick > 0)
								{
									pPlayBuffer(25, 0, 0);
									InputBox_SetText((Struct + 4), iUser5);
									InputBox_SetText((Struct + 240), iPassword5);
									RequestLogin(Struct);
								}
							}
							//****************
							if(Amount > 5)
							{
								//pDrawImage2(240, X + PosXMove + TransFormX(402), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
								RenderImageF(151, X + PosXMove + TransFormX(322), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
								//RenderBitmap(223, X + PosXMove + TransFormX(324), PosYMove, TransFormX(75), TransFormY(42), 0.f, 0.f, v23, v22, 0, 1, 0.0);
								//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(322), PosYMoveB, iUser6, TransFormX(57), 0, (LPINT)3, 0);
								MU_DrawTextOut( X + PosXMove + TransFormX(326), PosYMoveB, iUser6, TransFormX(65), 3, 0);
								if(CheckMouseIn(X + PosXMove + TransFormX(322), PosYMove, TransFormX(57), TransFormY(24)))
								{
									if (LeftClick > 0)
									{
										pPlayBuffer(25, 0, 0);
										InputBox_SetText((Struct + 4), iUser6);
										InputBox_SetText((Struct + 240), iPassword6);
										RequestLogin(Struct);
									}
								}
								//****************
								if(Amount > 6)
								{
									//pDrawImage2(240, X + PosXMove + TransFormX(480), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
									RenderImageF(151, X + PosXMove + TransFormX(380), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
									//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(380), PosYMoveB, iUser7, TransFormX(57), 0, (LPINT)3, 0);
									MU_DrawTextOut( X + PosXMove + TransFormX(384), PosYMoveB, iUser7, TransFormX(65), 3, 0);
									if(CheckMouseIn(X + PosXMove + TransFormX(380), PosYMove, TransFormX(57), TransFormY(24)))
									{
										if (LeftClick > 0)
										{
											pPlayBuffer(25, 0, 0);
											InputBox_SetText((Struct + 4), iUser7);
											InputBox_SetText((Struct + 240), iPassword7);
											RequestLogin(Struct);
										}
									}
									//****************
									if(Amount > 7)
									{
										//pDrawImage2(240, X + PosXMove + TransFormX(558), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
										RenderImageF(151, X + PosXMove + TransFormX(438), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
										//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(438), PosYMoveB, iUser8, TransFormX(57), 0, (LPINT)3, 0);
										MU_DrawTextOut( X + PosXMove + TransFormX(442), PosYMoveB, iUser8, TransFormX(65), 3, 0);
										if(CheckMouseIn(X + PosXMove + TransFormX(438), PosYMove, TransFormX(57), TransFormY(24)))
										{
											if (LeftClick > 0)
											{
												pPlayBuffer(25, 0, 0);
												InputBox_SetText((Struct + 4), iUser8);
												InputBox_SetText((Struct + 240), iPassword8);
												RequestLogin(Struct);
											}
										}
										//****************
										if(Amount > 8)
										{
											//pDrawImage2(240, X + PosXMove + TransFormX(636), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
											RenderImageF(151, X + PosXMove + TransFormX(496), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
											//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(496), PosYMoveB, iUser9, TransFormX(57), 0, (LPINT)3, 0);
											MU_DrawTextOut( X + PosXMove + TransFormX(500), PosYMoveB, iUser9, TransFormX(65), 3, 0);
											if(CheckMouseIn(X + PosXMove + TransFormX(496), PosYMove, TransFormX(57), TransFormY(24)))
											{
												if (LeftClick > 0)
												{
													pPlayBuffer(25, 0, 0);
													InputBox_SetText((Struct + 4), iUser9);
													InputBox_SetText((Struct + 240), iPassword9);
													RequestLogin(Struct);
												}
											}
											//****************
											if(Amount > 9)
											{
												//pDrawImage2(240, X + PosXMove + TransFormX(714), PosYMove, 60.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
												RenderImageF(151, X + PosXMove + TransFormX(554), PosYMove, TransFormX(57), TransFormY(24), 0.0, 0.0, 56, 23);
												//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(554), PosYMoveB, iUser10, TransFormX(57), 0, (LPINT)3, 0);
												MU_DrawTextOut( X + PosXMove + TransFormX(558), PosYMoveB, iUser10, TransFormX(65), 3, 0);
												if(CheckMouseIn(X + PosXMove + TransFormX(554), PosYMove, TransFormX(57), TransFormY(24)))
												{
													if (LeftClick > 0)
													{
														pPlayBuffer(25, 0, 0);
														InputBox_SetText((Struct + 4), iUser10);
														InputBox_SetText((Struct + 240), iPassword10);
														RequestLogin(Struct);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CLoginWin::Load()
{
	SetCompleteHook(0xE8, 0x006DEAD8, &CLoginWin::cSetPosition);//006DEAD8
	SetCompleteHook(0xE8, 0x006DEDD5, &CButtonAutoLogin);//CALL 006DEDDD//PUSH 06DEDD5//CALL 006EDD14
}//CALL 006E5DE4