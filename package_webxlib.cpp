#include <new>
#include "pch.h"

#define WEBXLUXLIB "WEBXLIB"

/**********************************************
CSOCKET userdata utility functions
**********************************************/
static webxlib::csocket* TO_csocket(luxZ* L, int index)
{
	webxlib::csocket* luasock = (webxlib::csocket*)L->ToUserdata(index);
	if (!luasock)
		L->TypeError(index, WEBXLUXLIB);

	return luasock;
}

static webxlib::csocket* CHECK_csocket(luxZ* L, int index)
{
	L->CheckType(index, LUA_TUSERDATA);

	webxlib::csocket* luasock = (webxlib::csocket*)L->CheckUserdata(index, WEBXLUXLIB);
	if (!luasock)
		L->TypeError(index, WEBXLUXLIB);

	return luasock;
}

static webxlib::csocket* PUSH_csocket(luxZ* L)
{
	webxlib::csocket* luasock = (webxlib::csocket*)L->NewUserdata(sizeof webxlib::csocket);

	L->GetMetatable(WEBXLUXLIB);
	L->SetMetatable(-2);

	return luasock;
}

/**********************************************
CSOCKET Lua class metamethods
**********************************************/
static LUXPACKAGE GC_csocket(luxZ* L)
{
	webxlib::csocket* luasock = TO_csocket(L, 1);
	delete luasock;

	return 0;
}

static LUXPACKAGE TOSTR_csocket(luxZ* L)
{
	char buff[32];

	sprintf_s(buff, "%p", TO_csocket(L, 1));

	L->PushFString("CSOCKET: 0x%p", buff);

	return 1;
}

/**********************************************
CSOCKET class functions
**********************************************/
static LUXPACKAGE CONSTRUCT_csocket(luxZ* L)
{
	csockdata bdata;
	bdata.address			= L->ToString(1);
	bdata.port				= L->ToString(2);
	bdata.dataprotocol	= L->CheckInt(3);
	bdata.ipprotocol		= L->CheckInt(4);

	webxlib::csocket* luasock = PUSH_csocket(L);
	new (luasock) webxlib::csocket(&bdata);

	return 1;
}

static LUXPACKAGE ISVALID_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	if (luasock->IsValid()) {
		L->PushNumber(1);
	} else {
		L->PushNumber(0);
	}

	return 1;
}

static LUXPACKAGE CHECKTYPE_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	if (luasock->CheckType()) {
		L->PushNumber(1);
	} else {
		L->PushNumber(0);
	}

	return 1;
}

static LUXPACKAGE SETSECURE_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	int arg = L->CheckInt(2);

	if (arg == 1) {
		luasock->SetSecure(true);
		L->PushNumber(1);
	} else if (arg == 0) {
		luasock->SetSecure(false);
		L->PushNumber(0);
	}

	return 1;
}

static LUXPACKAGE BIND_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	L->PushNumber(luasock->Bind());

	return 1;
}

static LUXPACKAGE LISTEN_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->Listen());

	return 1;
}
static LUXPACKAGE ACCEPT_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	webxlib::csocket* ret = luasock->Accept();
	if (ret->IsValid()) {
		webxlib::csocket* cluasock = PUSH_csocket(L);
		*cluasock = *ret;

		return 1;
	}

	return 0;
}

static LUXPACKAGE CONNECT_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->Connect());

	return 1;
}

static LUXPACKAGE SSLINIT_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	const char* cert = L->CheckString(2);
	const char* key = L->CheckString(3);

	L->PushNumber(luasock->SSL_Init(cert, key));

	return 1;
}

static LUXPACKAGE SSLBind_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SSLBind());

	return 1;
}

static LUXPACKAGE SSLConnect_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SSLConnect());

	return 1;
}

static LUXPACKAGE SSLACCEPT_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SSLAccept());

	return 1;
}

static LUXPACKAGE SSLWANTREAD_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SSLWantRead());

	return 1;
}

static LUXPACKAGE SSLWANTWRITE_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SSLWantWrite());

	return 1;
}

static LUXPACKAGE SELECTREADABLE_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SelectReadable({0,0}));

	return 1;
}

static LUXPACKAGE SELECTWRITEABLE_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	L->PushNumber(luasock->SelectWriteable({0,0}));

	return 1;
}

static LUXPACKAGE SETSOCKOPT_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	int lvl								 = L->CheckInt(2);
	int optname						 = L->CheckInt(3);
	const char* optval			 = L->CheckString(4);
	int oplen							 = L->CheckInt(5);

	L->PushNumber(luasock->SetSockOpt(lvl, optname, optval, oplen));

	return 1;
}

static LUXPACKAGE IOCTRLSOCK_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	int cmd								 = L->CheckInt(2);
	int argp							 = L->CheckInt(3);

	L->PushNumber(luasock->IOCtrlSocket(cmd, (u_long*)&argp));

	return 1;
}

static LUXPACKAGE SEND_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);
	const char* cmd				 = L->CheckString(2);
	int sz								 = L->CheckInt(3);

	L->PushNumber(luasock->Send(cmd, sz));

	return 1;
}

static LUXPACKAGE RECV_csocket(luxZ* L)
{
	webxlib::csocket* luasock = CHECK_csocket(L, 1);

	char buffer[1501];
	ZeroMemory(buffer, 1501);

	int got = luasock->Recv(buffer, 1500);
	if (got == CSOCKET_ERROR)
		return 0;

	L->PushString(buffer);

	return 1;
}

luxZ* catalyst = new luxZ(true);
static int garbc(lua_State* L) { catalyst->ReassignEnvironment(L); return GC_csocket(catalyst); }
static int tostc(lua_State* L) { catalyst->ReassignEnvironment(L); return TOSTR_csocket(catalyst); }
static int makec(lua_State* L) { catalyst->ReassignEnvironment(L); return CONSTRUCT_csocket(catalyst); }
static int isvdc(lua_State* L) { catalyst->ReassignEnvironment(L); return ISVALID_csocket(catalyst); }
static int chktp(lua_State* L) { catalyst->ReassignEnvironment(L); return CHECKTYPE_csocket(catalyst); }
static int stscr(lua_State* L) { catalyst->ReassignEnvironment(L); return SETSECURE_csocket(catalyst); }
static int bindc(lua_State* L) { catalyst->ReassignEnvironment(L); return BIND_csocket(catalyst); }
static int listc(lua_State* L) { catalyst->ReassignEnvironment(L); return LISTEN_csocket(catalyst); }
static int acptc(lua_State* L) { catalyst->ReassignEnvironment(L); return ACCEPT_csocket(catalyst); }
static int cnctc(lua_State* L) { catalyst->ReassignEnvironment(L); return CONNECT_csocket(catalyst); }
static int sslic(lua_State* L) { catalyst->ReassignEnvironment(L); return SSLINIT_csocket(catalyst); }
static int sslbc(lua_State* L) { catalyst->ReassignEnvironment(L); return SSLBind_csocket(catalyst); }
static int sslac(lua_State* L) { catalyst->ReassignEnvironment(L); return SSLACCEPT_csocket(catalyst); }
static int slwrc(lua_State* L) { catalyst->ReassignEnvironment(L); return SSLWANTREAD_csocket(catalyst); }
static int slwwc(lua_State* L) { catalyst->ReassignEnvironment(L); return SSLWANTWRITE_csocket(catalyst); }
static int selrc(lua_State* L) { catalyst->ReassignEnvironment(L); return SELECTREADABLE_csocket(catalyst); }
static int selwc(lua_State* L) { catalyst->ReassignEnvironment(L); return SELECTWRITEABLE_csocket(catalyst); }
static int ioctc(lua_State* L) { catalyst->ReassignEnvironment(L); return IOCTRLSOCK_csocket(catalyst); }
static int setsc(lua_State* L) { catalyst->ReassignEnvironment(L); return SETSOCKOPT_csocket(catalyst); }
static int sendc(lua_State* L) { catalyst->ReassignEnvironment(L); return SEND_csocket(catalyst); }
static int recvc(lua_State* L) { catalyst->ReassignEnvironment(L); return RECV_csocket(catalyst); }

static const luaL_Reg csocketmeta[] = {
	{"__gc",		garbc},
	{"__tostring",	tostc},
	{0, 0}
};

static const luaL_Reg csocketmembers[] = {
	{"New",				makec},
	{"IsValid",			isvdc},
	{"CheckType",     chktp},
	{"SetSecure",       stscr},
	{"Bind",			    bindc},
	{"Listen",			    listc},
	{"Accept",			acptc},
	{"Connect",			cnctc},
	{"SSL_Init",		    sslic},
	{"SSLBindSocket",	sslbc},
	{"SSLAccept",		    sslac},
	{"SSLWantRead",		slwrc},
	{"SSLWantWrite",	slwwc},
	{"SelectReadable",	selrc},
	{"SelectWriteable", selwc},
	{"IOCtrlSocket",	    ioctc},
	{"SetSockOpt",	    setsc},
	{"Send",			        sendc},
	{"Recv",			        recvc},
	{0,0}
};

LUXPACKAGE webxluxlib(luxZ* L)
{
	L->OpenLib(WEBXLUXLIB, csocketmembers, 0);

	L->NewMetatable(WEBXLUXLIB);

	L->OpenLib(NULL, csocketmeta, 0);
	L->PushLiteral("__index");
	L->PushValue(-3);
	L->Rawset(-3);

	L->PushLiteral("__metatable");
	L->PushValue(-3);
	L->Rawset(-3);

	L->Pop(1);

	return 1;
}