/*
      Free MultiHack V1 for Ukraine GTA 
      By DroidZero a.k.a NtKernelMC
      Discord: ntkernelmc
      Telegram: @droid_zero
*/

#include <Windows.h>
#include <stdio.h>
#include "sigscan.h"
#include "Utils.h"

bool OneShot = false;
bool DamagerX2 = false;
bool NoSpread = false;

BYTE OneShotKey = VK_DELETE;
BYTE DamagerX2Key = VK_END;
BYTE HealthKey = VK_HOME;
BYTE ArmorKey = VK_F10;
BYTE NoSpreadKey = VK_F5;

void CheatThread()
{
    while (true)
    {
        if (GetAsyncKeyState(OneShotKey))
        {
            OneShot ^= true;
            MessageBeep(MB_ICONHAND);
		}
        if (GetAsyncKeyState(DamagerX2Key))
        {
            DamagerX2Key ^= true;
            MessageBeep(MB_ICONHAND);
        }
        if (GetAsyncKeyState(HealthKey))
        {
            DWORD PEDSELF = *(DWORD*)0xB6F5F0;
            *(float*)(PEDSELF + 0x540) = 175.0f;
			MessageBeep(MB_ICONHAND);
		}
        if (GetAsyncKeyState(ArmorKey))
        {
            DWORD PEDSELF = *(DWORD*)0xB6F5F0;
            *(float*)(PEDSELF + 0x548) = 100.0f;
            MessageBeep(MB_ICONHAND);
        }
        if (GetAsyncKeyState(NoSpreadKey))
        {
            NoSpreadKey ^= true;
            if (NoSpread) SetSpreadMultiplier(0.00F);
            else SetSpreadMultiplier(100.00F);
            MessageBeep(MB_ICONHAND);
        }
        Sleep(150);
    }
}

int __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatThread, 0, 0, 0);
        break;
    }
    return 1;
}