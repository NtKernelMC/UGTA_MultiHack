#pragma once
#include <Windows.h>
#include <stdio.h>

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