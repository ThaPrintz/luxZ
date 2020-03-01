#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call,  LPVOID lpReserved)
{
    luxZ* st = nullptr;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        st = new luxZ();

        st->LoadFile("lux.lua");
        st->Pcall(0, LUA_MULTRET, 0);
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH){
        delete st;
    }

    return TRUE;
}

