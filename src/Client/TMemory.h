
#pragma once
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
 #define MEMORY4(value) *(unsigned int*)(value)
 #define MEMORY2(value) *(unsigned short*)(value)
 #define MEMORY1(value) *(unsigned char*)(value)
//Opcode const
#define _asm_call       0xE8
#define _asm_push       0x68
#define _asm_jmp        0xE9
#define _asm_jmps       0xEB//jmp short
#define _asm_jes        0x74
#define _asm_jnzs       0x75
#define _asm_jnz2       0x0F85//jmp short
#define _asm_nop        0x90
#define _asm_retn       0xC3
#define _asm_push_eax 0x50
bool ProcessReduceMemory();
DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize);
void ChangeAddress(DWORD Addr, DWORD AddrNew);
DWORD ReadMemory(const LPVOID lpAddress, LPVOID lpBuf, const UINT uSize);
void CommandLineToArg(char * Command, char*** argv);
DWORD SetByte(const LPVOID dwOffset, const BYTE btValue);
void SetByte1(DWORD offset,BYTE value);
void SetByte(DWORD offset,BYTE value);
DWORD GetByte(const LPVOID dwOffset, BYTE & btValue);
DWORD SetWord(const LPVOID dwOffset, const WORD wValue);
void SetWord(DWORD offset,WORD value) ;
DWORD GetWord(const LPVOID dwOffset, WORD & wValue);
DWORD SetDword(const LPVOID dwOffset, const DWORD dwValue);
DWORD GetDword(const LPVOID dwOffset, DWORD & dwValue);
DWORD SetFloat(const LPVOID dwOffset, float fValue);
void SetFloat(DWORD offset,float value);
DWORD GetFloat(const LPVOID dwOffset, float & fValue);
DWORD SetDouble(const LPVOID dwOffset, double dValue);
DWORD SetJmp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress);
DWORD SetJg(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress);
DWORD SetJa(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress);
DWORD SetOp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress, BYTE cmd);
DWORD SetPush(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress, const BYTE cmd);
DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue);
DWORD SetByte2(const LPVOID dwOffset, const BYTE btValue);
void MemoryCpy(DWORD offset,void* value,DWORD size);
DWORD SetHook(const LPVOID dwMyFuncOffset, const LPVOID dwJmpOffset, const BYTE cmd);
void HookOffset(DWORD my, DWORD tohook, BYTE type);
void SetNop(DWORD dwOffset,int Size);
void HookThis(DWORD dwMyFuncOffset,DWORD dwJmpOffset);
void Access(DWORD dwOffset, int t_size);
void HookAddress(DWORD dwAddrOffst, DWORD dwNewOffst, BYTE new_Cmd);
void SetCompleteHook(BYTE head, DWORD offset, ...); // OK;
void HookJump(DWORD Offset, LPVOID Function);
void SetDword(DWORD offset, DWORD value);
void HookCall(DWORD Offset, LPVOID Function);
float GetMemBuffer(DWORD *dwpAddress) ;
void UnProtect(DWORD *dwpAddress, float pBuffer) ;
void FatalError(char *szlog, ...);
void MsgError(char *szlog, ...);
bool FileExists(char * name);
void MessageBoxError(char *szlog, ...);
void ErrorMessageBox(char* message,...);
void WriteJmp(DWORD dwWriteOffset, DWORD dwDstJmp);
void MemorySet(DWORD offset, DWORD value, DWORD size) ;
char* ConvertModuleFileName(char* name);
//---------------------------------------------------------------------------


