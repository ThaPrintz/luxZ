#pragma once
#include "pch.h"

static int LoadLUXPackage(lua_State* L)
{
	luxZ* LL = new luxZ(L);

	const char* f1 = LL->ToString(1);

	LL->LoadFile(f1);
	LL->Pcall(0, LUA_MULTRET, 0);

	delete LL;

	return 1;
}

/*********************************
LUX environment initializer
*********************************/
LUXLIB_API luxZ* NewLUXInterface()
{
	luxZ* L = new luxZ();

	L->Register("loadluxpackage", LoadLUXPackage);

	csocketlib(L);

	return L;
}