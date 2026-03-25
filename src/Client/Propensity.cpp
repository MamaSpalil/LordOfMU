#include "stdafx.h"
//#include "MonsterLifeBar.h"
#include "Interface.h"
#include "Structure.h"
#include "MakePreviewCharSet.h"
#include "Protocol.h"
#include "TMemory.h"
//#include "LoadImages.h"
#include "Other.h"
VpPropensityInfo VpPropObj[65535];	// line : 180
//void PropensityDrawIcon()
//{
// 
//    // ----
//
//    VAngle Angle;
//    int PosX;
//	int PosY;
//	int ObjPreview;
//    float PropensityBarWitch = 40.0f;
//
//    // ----
// for( int ViewPort = 0; ViewPort < 75; ViewPort++ )
//   {
//	   DWORD ObjViewPortStruct = *(DWORD*)(PREVIEW_STRUCT) + (ViewPort * PREVIEW_STRUCT_SIZE); // 47c
//		ObjPreview = CS_GET_STRUCT(ViewPort, dword_754F52C);
//		lpViewObj lpObj	= &*(ObjectPreview*)ObjPreview;
//		VPPROPOBJ mp_gObj = &VpPropObj[lpObj->VPIndex];
//	
//
//	 Angle.X = lpObj->VecPosX;
//     Angle.Y = lpObj->VecPosY;
//     Angle.Z = lpObj->VecPosZ + lpObj->SelectZone + 100.0;
//      // ----
//
//      pGetPosFromAngle(&Angle, &PosX, &PosY);
//	  float Width = 15.0;
//	  float Height = 15.0;
//	  float ScaleX = 0.99;
//	  float ScaleY = 0.99;
//
//	  PosX -= (int)floor(PropensityBarWitch / (double)2.0);
//	  PosY -= 25.0;
//////////////////
/////Light////////
//////////////////
//	  if(mp_gObj->Number == 1)
//	  {
//	  pSetBlend(true);
//
//	  PrintImages(2531,PosX, PosY,Width,Height,0.0,0.0,0.875, 0.875, 1, 1, 0.0);
//	  }
//////////////////
/////Chaos////////
//////////////////
//	  else if (mp_gObj->Number == 2)
//	  {
//	   pSetBlend(true);
//
//	     PrintImages(2529,PosX, PosY,Width,Height,0.0,0.0,0.875, 0.875, 1, 1, 0.0);
//	  }
///////////////////
/////Darkness//////
//////////////////
//	  else if(mp_gObj->Number == 3)
//	  {
//	  pSetBlend(true);
//
//	    PrintImages(2530,PosX, PosY,Width,Height,0.0,0.0,0.875, 0.875, 1, 1, 0.0);
//
//	  }
//////////////////
/////Harmony//////
//////////////////
//	  else if (mp_gObj->Number == 4)
//	  {
//	  pSetBlend(true);
// 
//	    PrintImages(2532,PosX, PosY,Width,Height,0.0,0.0,0.875, 0.875, 1, 1, 0.0);
//	  }
//	  else
//	  {
//		  return;
//	  
//	  }
//
// }
//	pGLSwitch();
//}
//
