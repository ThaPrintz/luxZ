// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LUXLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LUXLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//extern LUXLIB_API int nluxlib;
//LUXLIB_API int fnluxlib(void);

#ifdef LUXLIB_EXPORTS
#define LUXLIB_API __declspec(dllexport)
#else
#define LUXLIB_API __declspec(dllimport)
#endif

class LUXLIB_API luxZ
{
public:
	luxZ();
	~luxZ();

	void Register(const char* name, int func);
	int LoadFile(const char* file);
	void OpenLib(const char* name, const luaL_Reg* t, int up);

	const char* PushFString(const char* fmt, char buff);
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