#include "stdafx.h"
#include "Fog.h"
#include "detours.h"
#include "Other.h"



RGBAStruct FogMapColor(int Map)
{
	RGBAStruct rgb;

	//rgb.r = 0.0f; rgb.g = 0.0f; rgb.b = 0.0f; rgb.a = 1.0f; return rgb;

	switch (Map)
	{
	case 0x0: // Lorencia
		{
		/*
		rgb.r = 0.2337f; rgb.g = 0.3f; rgb.b = 0.198f; rgb.a = 1.0f;
		*/
		rgb.r = 0.85f; rgb.g = 0.8025f; rgb.b = 0.2805f; rgb.a = 1.0f; 
		
			return rgb;
		} 
		break;

	case 0x1: // Dungeon
		{
			rgb.r = 0.83f; rgb.g = 0.7581f; rgb.b = 0.6142f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x2: // Devias
		{
			rgb.r = 0.6643f; rgb.g = 0.8158f; rgb.b = 0.91f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x3: // Noria
		{
			rgb.r = 0.3689f; rgb.g = 0.58f; rgb.b = 0.2552f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x4: // Losttower
		{
			rgb.r = 0.0f; rgb.g = 0.0f; rgb.b = 0.0f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x6: // Stadium
		{
			//rgb.r = 0.6643f; rgb.g = 0.8158f; rgb.b = 0.91f; rgb.a = 1.0f; 
			rgb.r = 0.56f; rgb.g = 0.4424f; rgb.b = 0.308f; rgb.a = 1.0f; return rgb;
		}

	case 0x7: // Atlans
		{
			rgb.r = 0.451f; rgb.g = 0.6478f; rgb.b = 0.82f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x8: // Tarkan
		{
			rgb.r = 0.83f; rgb.g = 0.667f; rgb.b = 0.3154f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0xA: // Icarus
		{
			rgb.r = 0.077f; rgb.g = 0.1318f; rgb.b = 0.22f; rgb.a = 1.0f; return rgb;
		} 
		break; 

	case 0x19: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break; 

	case 0x1A: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break; 

	case 0x1B: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x1C: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x1D: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x1E: // Kalima
		{
			rgb.r = 0.1736f; rgb.g = 0.3475f; rgb.b = 0.56f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x1F: // Valley of Loren
		{
			rgb.r = 0.39f; rgb.g = 0.3156f; rgb.b = 0.1872f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x20: // Land of Trial
		{
			rgb.r = 0.0f; rgb.g = 0.0f; rgb.b = 0.0f; rgb.a = 1.0f; return rgb;
		} 
		break; 

	case 0x21: // Aida
		{
			rgb.r = 0.3403f; rgb.g = 0.47f; rgb.b = 0.3008f; rgb.a = 1.0f; return rgb;
		} 
		break; 


	case 0x22: // CryWolf
		{
			rgb.r = 0.39f; rgb.g = 0.3156f; rgb.b = 0.1872f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x25: // Kanturu
		{
			rgb.r = 0.4f; rgb.g = 0.3364f; rgb.b = 0.188f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x26: // Kanturu
		{
			rgb.r = 0.18f; rgb.g = 0.1708f; rgb.b = 0.1494f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x2A: // Balgas Barrack
		{
			rgb.r = 0.47f; rgb.g = 0.1363f; rgb.b = 0.1363f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x33: // Elbeland
		{
			rgb.r = 0.76f; rgb.g = 0.76f; rgb.b = 0.76f; rgb.a = 1.0f; return rgb;
		} 
		break;  

	case 0x39: // Raklion
		{
			rgb.r = 0.6643f; rgb.g = 0.8158f; rgb.b = 0.91f; rgb.a = 1.0f; return rgb;
		} 
		break;

	case 0x3E: // Santa town
		{
			rgb.r = 0.6643f; rgb.g = 0.8158f; rgb.b = 0.91f; rgb.a = 1.0f; return rgb;
		}

	default:
		{
			rgb.r = 0.0f; rgb.g = 0.0f; rgb.b = 0.0f; rgb.a = 1.0f; return rgb;
		} 
		break;
	}
}
