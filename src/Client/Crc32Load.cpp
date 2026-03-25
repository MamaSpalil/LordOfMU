//#include "stdafx.h"
//#include "Crc32.h"
//#include "Crc32Load.h"
//
//
//int Crc32()
//{
//	//const char* PlayerF = "Data/Player/Player.bmd";
//	//const char* ItemF = "Data/Local/Item.bmd";
//	//const char* SkillF = "Data/Local/Skill.bmd";
//	//const char* MuF = "Mu.exe";
//	const char* ProtectF = "Data/Customs/Phoenix.bmp";
//	//Checksum Code
//	//unsigned long PlayerFCRC = 0xe91eb6e7;
//	//long ItemFCRC = 0x26f13b58;
//	//long SkillFCRC = 0x6701393b;
//	//unsigned long MuFCRC = 0xeeebd604;
//	unsigned long ProtectFCRC = 0x1ea7dc38;
//	//Files
//	const char* c_ErrorMsg = "The file inccorect:";
//	const char* c_CheckSumLog = "Logs.txt";
//
//	CCRC32 MyCRC32;
//	MyCRC32.Initialize();
//	unsigned long ulCRC1 = MyCRC32.FileCRC(ProtectF);
//	/*unsigned long ulCRC2 = MyCRC32.FileCRC(ItemF);
//	unsigned long ulCRC3 = MyCRC32.FileCRC(SkillF);*/
//	//unsigned long ulCRC1 = MyCRC32.FileCRC(MuF);
//	//unsigned long ulCRC2 = MyCRC32.FileCRC(ProtectF);
//	
//	if(ulCRC1 != ProtectFCRC)
//	{
//		char dateStr [9];
//		char timeStr [9];
//		_strdate(dateStr);
//		_strtime(timeStr);
//							
//		// Error.log
//		SYSTEMTIME t;
//		GetLocalTime(&t);
//		FILE* FileHwnd1;
//		FileHwnd1 = fopen(c_CheckSumLog,"a+");
//		fprintf(FileHwnd1,"[%02d.%02d.%02d][%02d:%02d:%02d] %s %s \n",t.wDay,t.wMonth,t.wYear,t.wHour,t.wMinute,t.wSecond,c_ErrorMsg,ProtectF);
//		
//		fclose(FileHwnd1);
//
//		ExitProcess(0);
//	}
//	//if(ulCRC2 != ItemFCRC)
//	//{
//	//	char dateStr [9];
//	//	char timeStr [9];
//	//	_strdate(dateStr);
//	//	_strtime(timeStr);
//	//						
//	//	// Error.log
//	//	FILE* FileHwnd1;
//	//	FileHwnd1 = fopen(c_CheckSumLog,"a+");
//	//	fprintf(FileHwnd1,"%s %s %s %s\n",dateStr,timeStr,c_ErrorMsg,ItemF);
//	//	fclose(FileHwnd1);
//
//	//	ExitProcess(0);
//	//}
//	//if(ulCRC3 != SkillFCRC)
//	//{
//	//	char dateStr [9];
//	//	char timeStr [9];
//	//	_strdate(dateStr);
//	//	_strtime(timeStr);
//	//						
//	//	// Error.log
//	//	FILE* FileHwnd1;
//	//	FileHwnd1 = fopen(c_CheckSumLog,"a+");
//	//	fprintf(FileHwnd1,"%s %s %s %s\n",dateStr,timeStr,c_ErrorMsg,ItemF);
//	//	fclose(FileHwnd1);
//
//	//	ExitProcess(0);
//	//}
//	//if(ulCRC1 != MuFCRC)
//	//{
//	//	char dateStr [9];
//	//	char timeStr [9];
//	//	_strdate(dateStr);
//	//	_strtime(timeStr);
//	//						
//	//	// Error.log
//	//	FILE* FileHwnd1;
//	//	FileHwnd1 = fopen(c_AHSLog,"a+");
//	//	fprintf(FileHwnd1,"%s %s %s %s\n",dateStr,timeStr,c_ErrorMsg,MuF);
//	//	fclose(FileHwnd1);
//
//	//	ExitProcess(0);
//	//}
//	//if(ulCRC2 != ProtectFCRC)
//	//{
//	//	char dateStr [9];
//	//	char timeStr [9];
//	//	_strdate(dateStr);
//	//	_strtime(timeStr);
//	//						
//	//	// Error.log
//	//	FILE* FileHwnd1;
//	//	FileHwnd1 = fopen(c_AHSLog,"a+");
//	//	fprintf(FileHwnd1,"%s %s %s %s\n",dateStr,timeStr,c_ErrorMsg,ProtectF);
//	//	fclose(FileHwnd1);
//
//	//	ExitProcess(0);
//	//}
//}