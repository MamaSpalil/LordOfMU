#ifndef __version_H
#define __version_H

#pragma once

#if defined (__HACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
	#define __SOFTWARE_VERSION 2,0501,1003,27001
	#define __SOFTWARE_VERSION_STR "2.0501.1003.27001"
	#define __SOFTWARE_VERSION_ABOUT _T("Lord of MU Hacker v2.0501")
	#define __SOFTWARE_PRODUCT_NAME "Lord of MU"
	#define __SOFTWARE_COPYRIGHT "2010 (c) Kalin Temelkov"
	#define __SOFTWARE_COMPANY_NAME "Kalin Temelkov"

	#define __CLICKER_DLL_NAME	"LordOfClicker.dll"
	#define __LOADER_DLL_NAME	"LordOfLoader.dll"
	#define __LORDOFMU_DLL_NAME "LordOfMU.dll"
	#define __LAUNCHER_EXE_NAME "LordOfMU.exe"
#elif defined(__CLICKER_STUFF__)
	#define __CLICKER_ELITE__
	#define __CLICKER_AVANTA__

	#define __SOFTWARE_VERSION 2,0600,1003,30002
	#define __SOFTWARE_VERSION_STR "2.0600.1003.30002"

	#define __SOFTWARE_VERSION_ABOUT _T("MUAutoClicker v2.0600 (AVANTA+ELITE)")
	#define __SOFTWARE_PRODUCT_NAME "MUAutoClicker (AVANTA+ELITE)"

	#define __SOFTWARE_COPYRIGHT "Copyright 2010 (c) Kalin Temelkov"
	#define __SOFTWARE_COMPANY_NAME "Kalin Temelkov"

	#define __CLICKER_DLL_NAME	"MUAutoClicker.dll"
	#define __LOADER_DLL_NAME	"MUAutoLoader.dll"
	#define __LORDOFMU_DLL_NAME "MUEliteClicker.dll"
	#define __LAUNCHER_EXE_NAME "MUAutoClicker.exe"
#elif defined(__ANTIHACK_STUFF__)
	#define __SOFTWARE_VERSION 2,0502,1002,09000
	#define __SOFTWARE_VERSION_STR "2.0502.1002.09000"
	#define __SOFTWARE_VERSION_ABOUT _T("Lord of MU Anticheat v2.0502")
	#define __SOFTWARE_PRODUCT_NAME "Lord of MU"
	#define __SOFTWARE_COPYRIGHT "2010 (c) Kalin Temelkov"
	#define __SOFTWARE_COMPANY_NAME "Kalin Temelkov"

	#define __CLICKER_DLL_NAME	"LordOfClicker.dll"
	#define __LOADER_DLL_NAME	"LordOfLoader.dll"
	#define __LORDOFMU_DLL_NAME "LordOfMU.dll"
	#define __LAUNCHER_EXE_NAME "LordOfMU.exe"
#else
	#error "Unknown software branch!"
#endif

#endif // __version_H
