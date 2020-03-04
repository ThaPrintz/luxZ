#pragma once
#include <lua.hpp>

#ifdef LUXLIB_EXPORTS
#define LUXLIB_API __declspec(dllexport)
#else
#define LUXLIB_API __declspec(dllimport)
#endif

typedef LUXLIB_API const int LUXPACKAGE;
static LUXPACKAGE CSOCKS = 1;

class LUXLIB_API luxZ
{
public:
	luxZ();
	luxZ(lua_State* L);
	luxZ(int EMPTY);
	virtual ~luxZ();

	void ReassignEnvironment(lua_State* L);
	void CloseEnvironment();

	void Register(const char* name, int(*)(lua_State* L))
	int LoadFile(const char* file);
	void OpenLib(const char* name, const luaL_Reg* t, int up);

	const char* PushFString(const char* fmt, char* buff);
	void PushString(const char* str);
	void PushLiteral(const char* str);
	void PushValue(int index);
	void PushNumber(int num);

	void Pop(int index);
	int Pcall(int nargs, int nresults, int errfunc);
	void Rawset(int index);

	const char* CheckString(int index);
	int CheckInt(int index);
	void CheckType(int index, int type);

	int NewMetatable(const char* name);
	int SetMetatable(int index);
	void GetMetatable(const char* name);

	void* ToUserdata(int index);
	void* NewUserdata(size_t size);
	void* CheckUserdata(int index, const char* name);

	int TypeError(int index, const char* name);

	const char* ToString(int index);
protected:
	lua_State* lux = nullptr;
};

static int LoadLUXPackage(lua_State *L){
	luxZ* LL = new luxZ(L);
	
	const char* f1 = LL->ToString(1);

	LL->LoadFile(f1);
	LL->Pcall(0, LUA_MULTRET, 0);

	return 1;
}

/******************************
LUX Package initializer functions
******************************/
LUXLIB_API LUXPACKAGE csocketlib(luxZ* L);

/*********************************
LUX environment initializer
*********************************/
LUXLIB_API luxZ* NewLUXInterface()
{
	luxZ* L = new luxZ();

	L->Register("loadluxpackage", LoadLUXPackage);

}