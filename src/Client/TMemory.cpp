#include "TMemory.h"
#include <stdio.h>
//---------------------------------------------------------------------------

// Thanks to S@nek (BoR)
//---------------------------------------------------------------------------

void SetCompleteHook(BYTE head, DWORD offset, ...) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 5, PAGE_EXECUTE_READWRITE, &OldProtect);

	if (head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset + 1;

	*(DWORD*)(offset + 1) = (*function) - (offset + 5);

	VirtualProtect((void*)offset, 5, OldProtect, &OldProtect);
}
void MemorySet(DWORD offset, DWORD value, DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	memset((void*)offset, value, size);

	VirtualProtect((void*)offset, size, OldProtect, &OldProtect);
}
void ChangeAddress(DWORD Addr, DWORD AddrNew)
{

	DWORD OldProtect;
	VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	__asm
	{
		MOV EAX, Addr;
		MOV EDX, AddrNew;
		MOV DWORD PTR DS : [EAX], EDX;
	}
	VirtualProtect((LPVOID)Addr, 4, OldProtect, &OldProtect);
}
DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize)
{
    DWORD dwErrorCode   = 0;
    DWORD dwOldProtect  = 0;
	// ----
    int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, & dwOldProtect);
	// ----
    if ( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    memcpy(lpAddress, lpBuf, uSize);
	// ----
    DWORD dwBytes   = 0;
	// ----
    iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, & dwBytes);
	// ----
    if ( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    return 0x00;
}
//---------------------------------------------------------------------------
void CommandLineToArg(char * Command, char*** argv)
{
	if (!Command) { return; }

	bool	Quote			=	false;
	bool	Content			=	false;
	size_t	CommandLen		=	strlen(Command);
	int		NumCommands		=	1;
	size_t	I				=	0;

	for (I=0; I < CommandLen; I++)
	{
		if (Command[I] == '"')
		{
			Quote			=	!Quote;
			Content			=	true;
		}
		else if (Command[I] == ' ')
		{
			if (!Quote&&Content)
			{
				Content		=	false;

				NumCommands++;
			}
		}
		else
		{
			Content			=	true;
		}
	}

	*argv = new char*[NumCommands];

	size_t StrBegin		=	0;
	size_t StrLen		=	0;
	size_t CommandNum	=	0;

	for (I=0; I < CommandLen; I++)
	{
		if (Command[I] == '"')
		{
			Quote = !Quote;
			Content			=	true;
		}
		else if (Command[I] == ' ')
		{
			if ( !Quote && Content)
			{
				Content		=	false;

				StrLen = I - StrBegin;
				if (Command[StrBegin] == '"')
				{
					StrBegin++;
					StrLen--;
				}
				if (Command[StrBegin+StrLen-1] == '"')
				{
					StrLen-=1;
				}
				char* Argument = 0;
				Argument = new char[StrLen+1];
				memcpy(Argument,Command + StrBegin,StrLen);
				memset(Argument+StrLen,0,1);

				(*argv)[CommandNum] = Argument;		

				CommandNum++;
				StrBegin = I+1;
			}
			else if ( !Quote )
			{
				StrBegin++;
			}
		}
		else
		{
			Content			=	true;
		}
	}
	StrLen = I - StrBegin;
	if (Command[StrBegin] == '"')
	{
		StrBegin++;
		StrLen--;
	}
	if (Command[StrBegin+StrLen-1] == '"')
	{
		StrLen-=1;
	}
	char* Argument = 0;
	Argument = new char[StrLen+1];
	memcpy(Argument,Command + StrBegin,StrLen);
	memset(Argument+StrLen,0,1);

	(*argv)[CommandNum] = Argument;
}
DWORD ReadMemory(const LPVOID lpAddress, LPVOID lpBuf, const UINT uSize)
{
    DWORD dwErrorCode   = 0;
    DWORD dwOldProtect  = 0;
	// ----
    int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, & dwOldProtect);
	// ----
    if ( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    memcpy(lpBuf, lpAddress, uSize);
	// ----
    DWORD dwBytes   = 0;
	// ----
    iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, & dwBytes);
	// ----
    if ( iRes == 0 )
    {
        dwErrorCode = GetLastError();
        return dwErrorCode;
    }
	// ----
    return 0x00;
}
//---------------------------------------------------------------------------

DWORD SetByte(const LPVOID dwOffset, const BYTE btValue)
{
	return WriteMemory(dwOffset, (LPVOID) & btValue, sizeof(BYTE));
}
void SetByte1(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}
void SetByte(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}

//---------------------------------------------------------------------------

DWORD GetByte(const LPVOID dwOffset, BYTE & btValue)
{
	return ReadMemory(dwOffset, (LPVOID) btValue, sizeof(BYTE));
}
//---------------------------------------------------------------------------

DWORD SetWord(const LPVOID dwOffset, const WORD wValue)
{
	return WriteMemory(dwOffset, (LPVOID) wValue, sizeof(WORD));
}
void SetWord(DWORD offset,WORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,2,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(WORD*)(offset)=value;

	VirtualProtect((void*)offset,2,OldProtect,&OldProtect);
}
//---------------------------------------------------------------------------

DWORD GetWord(const LPVOID dwOffset, WORD & wValue)
{
	return ReadMemory(dwOffset, (LPVOID) wValue, sizeof(WORD));
}
//---------------------------------------------------------------------------

DWORD SetDword(const LPVOID dwOffset, const DWORD dwValue)
{
	return WriteMemory(dwOffset, (LPVOID) dwValue, sizeof(DWORD));
}
//---------------------------------------------------------------------------

DWORD GetDword(const LPVOID dwOffset, DWORD & dwValue)
{
	return ReadMemory(dwOffset, (LPVOID) dwValue, sizeof(DWORD));
}
DWORD SetFloat(const LPVOID dwOffset, float fValue)
{
	return WriteMemory(dwOffset, & fValue, sizeof(float));
}
void SetFloat(DWORD offset,float value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(float*)(offset)=value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}
//---------------------------------------------------------------------------

DWORD GetFloat(const LPVOID dwOffset, float & fValue)
{
	return ReadMemory(dwOffset, & fValue, sizeof(float));
}
//---------------------------------------------------------------------------

DWORD SetDouble(const LPVOID dwOffset, double dValue)
{
	return WriteMemory(dwOffset, & dValue, sizeof(double));
}
//---------------------------------------------------------------------------

DWORD SetJmp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress)
{
	BYTE btBuf[5];
	DWORD dwShift	= (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 5;
	// ----
	btBuf[0]	= 0xE9;
	memcpy( (LPVOID) & btBuf[1], (LPVOID) & dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwEnterFunction, (LPVOID) btBuf, sizeof(btBuf));
}
//---------------------------------------------------------------------------

DWORD SetJg(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress)
{
	BYTE btBuf[6];
	DWORD dwShift	= (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 6;
	// ----
	btBuf[0]	= 0x0F;
	btBuf[1]	= 0x8F;
	memcpy( (LPVOID) & btBuf[2], (LPVOID) & dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwEnterFunction, (LPVOID) btBuf, sizeof(btBuf));
}
//---------------------------------------------------------------------------

DWORD SetJa(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress)
{
	BYTE btBuf[6];
	DWORD dwShift	= (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 6;
	// ----
	btBuf[0]	= 0x0F;
	btBuf[1]	= 0x87;
	memcpy( (LPVOID) & btBuf[2], (LPVOID) & dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwEnterFunction, (LPVOID) btBuf, sizeof(btBuf));
}
//---------------------------------------------------------------------------
DWORD SetByte2(const LPVOID dwOffset, const BYTE btValue)
{
	return WriteMemory(dwOffset, (LPVOID) & btValue, sizeof(BYTE));
}
DWORD SetOp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress, const BYTE cmd)
{
	BYTE btBuf[5];
	DWORD dwShift	= (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 5;
	// ----
	btBuf[0]		= cmd;
	memcpy( (LPVOID) & btBuf[1], (LPVOID) & dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwEnterFunction, (LPVOID) btBuf, sizeof(btBuf));
}
void MemoryCpy(DWORD offset,void* value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memcpy((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}
//---------------------------------------------------------------------------
char* ConvertModuleFileName(char* name) // OK
{
	static char buff[MAX_PATH]={0};

	for(int n=strlen(name);n > 0;n--)
	{
		if(name[n] == '\\')
		{
			strcpy_s(buff,sizeof(buff),&name[(n+1)]);
			break;
		}
	}

	return buff;
}
DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue)
{
	BYTE * lpBuf	= new BYTE[wCount];
	// ----
	memset(lpBuf, btValue, wCount);
	// ----
	return WriteMemory( dwAddress, (LPVOID) lpBuf, wCount);
}
//---------------------------------------------------------------------------

DWORD SetHook(const LPVOID dwMyFuncOffset, const LPVOID dwJmpOffset, const BYTE cmd)
{
	BYTE btBuf[5];
	// ----
	DWORD dwShift	= (ULONG_PTR)dwMyFuncOffset - ( (ULONG_PTR)dwJmpOffset + 5 );
	// ----
	btBuf[0] = cmd;
	// ----
	memcpy( (LPVOID) & btBuf[1], (LPVOID) & dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwJmpOffset, (LPVOID) btBuf, sizeof(btBuf));
}
void HookOffset(DWORD my, DWORD tohook, BYTE type)
{
	*(BYTE*)tohook = type;
	*(DWORD*)(tohook+1) = my - (tohook+5);
};
//---------------------------------------------------------------------------

void HookThis(DWORD dwMyFuncOffset,DWORD dwJmpOffset)
{
	*(DWORD*)(dwJmpOffset+1) = dwMyFuncOffset-(dwJmpOffset+5);
}
//---------------------------------------------------------------------------
void Access(DWORD dwOffset, int t_size)
{
        DWORD OldProtect;
        VirtualProtect((LPVOID)dwOffset, t_size, PAGE_EXECUTE_READWRITE, &OldProtect);
}
//---------------------------------------------------------------------------
void HookAddress(DWORD dwAddrOffst, DWORD dwNewOffst, BYTE new_Cmd)
{//Хук процедуры
        Access(dwAddrOffst, 5);
 
        if(new_Cmd)
                memset((void*)dwAddrOffst, new_Cmd, 1);
       
        *(DWORD*)(dwAddrOffst+1)=dwNewOffst-(dwAddrOffst+5);
}
void HookJump(DWORD Offset, LPVOID Function)
{
	*(BYTE*) (Offset) = 0xE9;
	*(DWORD*) (Offset + 1) = (DWORD) Function - (Offset + 5);
}
void SetDword(DWORD offset, DWORD value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(DWORD*)(offset) = value;

	VirtualProtect((void*)offset, 4, OldProtect, &OldProtect);
}
void HookCall(DWORD Offset, LPVOID Function)
{
	*(BYTE*) (Offset) = 0xE8;
	*(DWORD*) (Offset + 1) = (DWORD) Function - (Offset + 5);
}
void SetNop(DWORD Offset,int Size)
{
	for (int i = 0; i < Size; i++)
	{
		*(BYTE*) (Offset + i) = 0x90;
	}
}
float GetMemBuffer(DWORD *dwpAddress) 
{
	float pBuffer;
	if (ReadProcessMemory(GetCurrentProcess(), dwpAddress, (float*)&pBuffer, 4, 0) == TRUE) 
		return pBuffer;
	else return 0;
}
void UnProtect(DWORD *dwpAddress, float pBuffer) 
{
	DWORD dwOldProtect = 0;
	HANDLE hWnd = GetCurrentProcess();
	VirtualProtectEx(hWnd, dwpAddress, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hWnd, dwpAddress, (void*)&pBuffer, 4, 0);
	VirtualProtectEx(hWnd, dwpAddress, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
}
void FatalError(char *szlog, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;
	va_start(pArguments, szlog);
	vsprintf_s(szBuffer, szlog, pArguments);
	va_end(pArguments);
	MessageBox(NULL, szBuffer, "[Main] Fatal Error", MB_OK | MB_APPLMODAL);
	ExitProcess(0);
}

void MsgError(char *szlog, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;
	va_start(pArguments, szlog);
	vsprintf_s(szBuffer, szlog, pArguments);
	va_end(pArguments);
	MessageBox(NULL, szBuffer, "[Main] Fatal Error", MB_OK | MB_APPLMODAL);
}

bool FileExists(char * name)
{
	if (CreateFile(name, 0, 0, 0, OPEN_EXISTING, 0, 0) == INVALID_HANDLE_VALUE)
		return false;
	return true;
}
void MessageBoxError(char *szlog, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;
	va_start(pArguments, szlog);
	vsprintf(szBuffer, szlog, pArguments);
	va_end(pArguments);
	MessageBox(NULL, szBuffer, "error", MB_OK | MB_APPLMODAL);
}
void ErrorMessageBox(char* message,...) // OK
{
	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(0,buff,"Error",MB_OK | MB_ICONERROR);
}
void Protect(DWORD dwAddr, DWORD size, DWORD dwProtection) {
	DWORD old = 0;
	VirtualProtect((LPVOID)dwAddr, size, dwProtection, &old);
}

DWORD UnProtect(DWORD dwAddr, DWORD size) {
	DWORD old = 0;
	VirtualProtect((LPVOID)dwAddr, size, PAGE_EXECUTE_READWRITE, &old);
	return old;
}
void SetBytes(DWORD dwOffset, BYTE btValue) {
	__try {
		DWORD dwP = UnProtect(dwOffset, 1);
		*(BYTE*)(dwOffset)=btValue;
		Protect(dwOffset, 1, dwP);
	}
	__finally{}
}
void WriteJmp(DWORD dwWriteOffset, DWORD dwDstJmp) 
{
	if(dwWriteOffset)	{
		__try {
			DWORD dwP = UnProtect(dwWriteOffset, 10);
			SetBytes(dwWriteOffset, 0xE9);
			HookThis(dwDstJmp, dwWriteOffset);
			Protect(dwWriteOffset, 10, dwP);
		}
		__finally{}
	}
}