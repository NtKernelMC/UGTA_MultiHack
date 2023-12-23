#pragma once
#define LOG_NAME "SigTest.log"
#define HACK_VER "V2"
#include <Windows.h>
#include <stdio.h>
#include <direct.h>
#include <string>
#include <algorithm>
#include "WepTypes.h"
#include "CVector.h"
#include "sigscan.h"

bool findStringIC(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
	return (it != strHaystack.end());
}

void __stdcall LogInFile(std::string log_name, const char* log, ...)
{
	char hack_dir[600]; memset(hack_dir, 0, sizeof(hack_dir));
	_getcwd(hack_dir, sizeof(hack_dir)); char new_dir[600];
	memset(new_dir, 0, sizeof(new_dir));
	sprintf(new_dir, "%s\\%s", hack_dir, log_name.c_str());
	static bool once = false; if (!once)
	{
		FILE* hFile = fopen(new_dir, "rb");
		if (hFile) { fclose(hFile); DeleteFileA(new_dir); }
		once = true;
	}
	FILE* hFile = fopen(new_dir, "a+");
	if (hFile)
	{
		time_t t = std::time(0); tm* now = std::localtime(&t);
		char tmp_stamp[600]; memset(tmp_stamp, 0, sizeof(tmp_stamp));
		sprintf(tmp_stamp, "[%d:%d:%d]", now->tm_hour, now->tm_min, now->tm_sec);
		strcat(tmp_stamp, std::string(" " + std::string(log)).c_str());
		va_list arglist; va_start(arglist, log); vfprintf(hFile, tmp_stamp, arglist);
		va_end(arglist); fclose(hFile);
	}
}

void SetSpreadMultiplier(float fltMultiplier)
{
    *reinterpret_cast<float*>(0x008D6114) = fltMultiplier / 20.00F;
}

DWORD MakeJump(const DWORD jmp_address, const DWORD hookAddr, BYTE* prologue, const size_t prologue_size)
{
	DWORD old_prot = 0x0;
	if (prologue == nullptr) return 0x0;
	VirtualProtect((void*)jmp_address, prologue_size, PAGE_EXECUTE_READWRITE, &old_prot);
	memcpy(prologue, (void*)jmp_address, prologue_size);
	BYTE addrToBYTEs[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	DWORD JMPBYTEs = (hookAddr - jmp_address - 5);
	memcpy(&addrToBYTEs[1], &JMPBYTEs, 4);
	memcpy((void*)jmp_address, addrToBYTEs, 5);
	PVOID Trampoline = VirtualAlloc(0, (5 + (prologue_size - 5)), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (Trampoline == nullptr) return 0x0;
	BYTE TrampolineBYTEs[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	if (prologue_size > 5)
	{
		BYTE nop[] = { 0x90 };
		for (BYTE x = 0; x < (prologue_size - 5); x++) memcpy((void*)(jmp_address + 0x5 + x), nop, 1);
		memcpy(Trampoline, &prologue[3], (prologue_size - 3));
		DWORD Delta = (jmp_address + prologue_size) - (((DWORD)Trampoline + (prologue_size - 3)) + 5);
		memcpy(&TrampolineBYTEs[1], &Delta, 4);
		memcpy((void*)((DWORD)Trampoline + (prologue_size - 3)), TrampolineBYTEs, 5);
	}
	else
	{
		DWORD Delta = (jmp_address + prologue_size) - ((DWORD)Trampoline + 5);
		memcpy(&TrampolineBYTEs[1], &Delta, 4);
		memcpy(Trampoline, TrampolineBYTEs, 5);
	}
	VirtualProtect((void*)jmp_address, prologue_size, old_prot, 0);
	return (DWORD)Trampoline;
}

bool RestorePrologue(DWORD addr, BYTE* prologue, size_t prologue_size)
{
	if (prologue == nullptr) return false;
	DWORD old_prot = 0;
	VirtualProtect((void*)addr, prologue_size, PAGE_EXECUTE_READWRITE, &old_prot);
	memcpy((void*)addr, prologue, prologue_size);
	VirtualProtect((void*)addr, prologue_size, old_prot, &old_prot);
	return true;
}