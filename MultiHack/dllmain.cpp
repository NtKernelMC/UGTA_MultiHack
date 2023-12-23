/*
      Free MultiHack V1 for Ukraine GTA 
      By DroidZero a.k.a NtKernelMC
      Discord: ntkernelmc
      Telegram: @droid_zero
*/
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "Utils.h"

void InstallHooks();
BYTE firstPrologue[5];
BYTE secondPrologue[6];

bool OneShot = false;
bool DamagerX2 = false;
bool NoSpread = false;

// Pacany, tut mozete izmenit knopki )0)
BYTE OneShotKey = VK_DELETE;
BYTE DamagerX2Key = VK_END;
BYTE HealthKey = VK_HOME;
BYTE ArmorKey = VK_F10;
BYTE NoSpreadKey = VK_F5;

typedef void(__thiscall* ptrSendBulletSyncFire)(void* ECX, eWeaponType weaponType, CVector& vecStart, CVector& vecEnd, float fDamage, BYTE ucHitZone, void* pRemoteDamagedPlayer);
ptrSendBulletSyncFire callSendBulletSyncFire = nullptr;
void __fastcall SendBulletSyncFire(void* ECX, void* EDX, eWeaponType weaponType, CVector& vecStart, CVector& vecEnd, float fDamage, BYTE ucHitZone, void* pRemoteDamagedPlayer)
{
    RestorePrologue((DWORD)SendBulletSyncFire, secondPrologue, 6);
    if (pRemoteDamagedPlayer != nullptr) // if we hit a player
    {
        if (OneShot)
        {
            // 30 bullets per one shot (fucking death)
            for (BYTE x = 0; x < 30; x++) callSendBulletSyncFire(ECX, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
            MakeJump((DWORD)callSendBulletSyncFire, (DWORD)&SendBulletSyncFire, secondPrologue, 6);
            return;
        }
        if (DamagerX2)
        {
            // x2 bullet per shot
            for (BYTE x = 0; x < 2; x++) callSendBulletSyncFire(ECX, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
            MakeJump((DWORD)callSendBulletSyncFire, (DWORD)&SendBulletSyncFire, secondPrologue, 6);
            return;
        }
    }
    MakeJump((DWORD)callSendBulletSyncFire, (DWORD)&SendBulletSyncFire, secondPrologue, 6);
    callSendBulletSyncFire(ECX, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
}

typedef bool(__thiscall* ptrSendPacket)(void* ECX, unsigned char ucPacketID, void* bitStream, int packetPriority, int packetReliability, int packetOrdering);
ptrSendPacket callSendPacket = nullptr;
bool __fastcall SendPacket(void* ECX, void* EDX, unsigned char ucPacketID, void* bitStream, int packetPriority, int packetReliability, int packetOrdering)
{
    RestorePrologue((DWORD)callSendPacket, firstPrologue, 5);
    if (ucPacketID == 91)
    {
        MakeJump((DWORD)callSendPacket, (DWORD)&SendPacket, firstPrologue, 5);
        return true; // shout up anticheat
    }
    bool rslt = callSendPacket(ECX, ucPacketID, bitStream, packetPriority, packetReliability, packetOrdering);
    MakeJump((DWORD)callSendPacket, (DWORD)&SendPacket, firstPrologue, 5);
    return rslt;
}

void CheckHooks()
{
    while (true)
    {
        if (callSendPacket != nullptr && callSendBulletSyncFire != nullptr)
        {
            if (*(BYTE*)callSendPacket != 0xE9 || *(BYTE*)callSendBulletSyncFire != 0xE9)
            {
				LogInFile(LOG_NAME, "[LOG] Hooks are broken! Reinstalling...\n");
                InstallHooks(); Sleep(60000);
			}
        }
        Sleep(1000);
    }
}

void InstallHooks()
{
    SigScan scan;
    while (!GetModuleHandleA("client.dll")) Sleep(1000);

    callSendPacket = (ptrSendPacket)scan.FindPattern("netc.dll",
    "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xF0\x56\x57\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x8B\xF1\x89\xB5\x00\x00\x00\x00\x8B\x7D\x0C",
    "xxxxxx????xx????xxx????x????xxxxxxxxxxxxx????xxxx????xxx"); 
    if (callSendPacket != nullptr)
    {
        LogInFile(LOG_NAME, "[LOG] Found address from signature to SendPacket! 0x%X\n", (DWORD)callSendPacket);
        MakeJump((DWORD)callSendPacket, (DWORD)&SendPacket, firstPrologue, 5);
    }
    else
    {
        LogInFile(LOG_NAME, "[ERROR] Can`t find address for SendPacket signature!");
        TerminateProcess(GetCurrentProcess(), 0);
    }

    callSendBulletSyncFire = (ptrSendBulletSyncFire)scan.FindPattern("client.dll",
    "\x55\x8B\xEC\x56\x8B\xF1\x8B\x00\x00\x00\x00\x00\x57", "xxxxxxx?????x"); 
    if (callSendBulletSyncFire != nullptr)
    {
        LogInFile(LOG_NAME, "[LOG] Found address from signature to SendBulletSyncFire! 0x%X\n", (DWORD)callSendBulletSyncFire);
        MakeJump((DWORD)callSendBulletSyncFire, (DWORD)&SendBulletSyncFire, secondPrologue, 6);
    }
    else
    {
        LogInFile(LOG_NAME, "[ERROR] Can`t find address for SendBulletSyncFire signature!");
        TerminateProcess(GetCurrentProcess(), 0);
    }

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheckHooks, 0, 0, 0);
}

void CheatThread()
{
    InstallHooks();
    while (true)
    {
        if (GetAsyncKeyState(OneShotKey))
        {
            OneShot ^= true;
            MessageBeep(MB_ICONHAND);
            Sleep(1000);
		}
        if (GetAsyncKeyState(DamagerX2Key))
        {
            DamagerX2Key ^= true;
            MessageBeep(MB_ICONHAND);
            Sleep(1000);
        }
        if (GetAsyncKeyState(HealthKey))
        {
            DWORD PEDSELF = *(DWORD*)0xB6F5F0;
            *(float*)(PEDSELF + 0x540) = 175.0f;
			MessageBeep(MB_ICONHAND);
            Sleep(1000);
		}
        if (GetAsyncKeyState(ArmorKey))
        {
            DWORD PEDSELF = *(DWORD*)0xB6F5F0;
            *(float*)(PEDSELF + 0x548) = 100.0f;
            MessageBeep(MB_ICONHAND);
            Sleep(1000);
        }
        if (GetAsyncKeyState(NoSpreadKey))
        {
            NoSpreadKey ^= true;
            if (NoSpread) SetSpreadMultiplier(0.00F);
            else SetSpreadMultiplier(100.00F);
            MessageBeep(MB_ICONHAND);
            Sleep(1000);
        }
        Sleep(150);
    }
}

int __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LogInFile(LOG_NAME, "[LOG] MultiHack %s by DroidZero injected!\n", HACK_VER);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatThread, 0, 0, 0);
        break;
    }
    return 1;
}