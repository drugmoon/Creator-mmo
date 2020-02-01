#include "StdAfx.h"
#include "ScriptLua.h"
#include "ScriptLuaNpcBind.h"
#include "scriptluaplayerbind.h"
#include "ScriptLuaMonsterBind.h"
#include "ScriptLuaGuildBind.h"
#include "ScriptLuaMapBind.h"
#include "ScriptLuaItemBind.h"
#include "ScriptLuaSlaveBind.h"
#include "GameServerLib.h"
#include "GuildGuild.h"
#include "ManagerGuild.h"
#include "PlayMap.h"
#include "PlaySlave.h"
#include "ManagerMap.h"
#include "GameSession.h"
#include "ManagerGroup.h"
#include "ManagerMail.h"
#include "DomainPlay.h"
#include "DomainGroup.h"
#include "GroupGroup.h"
#include "ManagerItemDef.h"
#include "ManagerFly.h"
#include "ManagerChargeDart.h"
#include "ManagerAutoRun.h"

extern "C" {
#include "cjson/lua_cjson.h"
}

static const char *progname = "lua";

static void l_message (const char *pname, const char *msg,const char* function) {
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s-%s\n", msg,function);
  fflush(stderr);
}


static int report (lua_State *L, int status,const char* function) {
  if (status && !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    l_message(progname, msg,function);
    lua_pop(L, 1);
  }
  return status;
}


static int traceback (lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}


static int docall (lua_State *L, int narg, int clear) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  //signal(SIGINT, laction);
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  //signal(SIGINT, SIG_DFL);
  lua_remove(L, base);  /* remove traceback function */
  /* force a complete garbage collection in case of errors */
  if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
  return status;
}


static void print_version (void) {
  l_message(NULL, LUA_RELEASE "  " LUA_COPYRIGHT,"");
}


static int getargs (lua_State *L, char **argv, int n) {
  int narg;
  int i;
  int argc = 0;
  while (argv[argc]) argc++;  /* count total number of arguments */
  narg = argc - (n + 1);  /* number of arguments to the script */
  luaL_checkstack(L, narg + 3, "too many arguments to script");
  for (i=n+1; i < argc; i++)
    lua_pushstring(L, argv[i]);
  lua_createtable(L, narg, n + 1);
  for (i=0; i < argc; i++) {
    lua_pushstring(L, argv[i]);
    lua_rawseti(L, -2, i - n);
  }
  return narg;
}


static int dostring (lua_State *L, const char *s, const char *name) {
  int status = luaL_loadbuffer(L, s, strlen(s), name) || docall(L, 0, 1);
  return report(L, status,"");
}


static int dolibrary (lua_State *L, const char *name) {
  lua_getglobal(L, "require");
  lua_pushstring(L, name);
  return report(L, docall(L, 1, 1),"");
}
static void luaL_findfunction (lua_State *L, int idx, const char *fname) {
  const char *e;
  lua_pushvalue(L, idx);
  do {
    e = strchr(fname, '.');
    if (e == NULL) e = fname + strlen(fname);
    lua_pushlstring(L, fname, e - fname);
    lua_rawget(L, -2);
    if (lua_isnil(L, -1)) {  /* no such field? */
		lua_remove(L, -2);  /* remove previous table */
		return;
    }
    else if (lua_istable(L, -1)) {  /* field has a non-table value? */
		lua_remove(L, -2);  /* remove previous table */
		fname = e + 1;
    }
	else if (lua_isfunction(L,-1)) {
		lua_remove(L, -2);  /* remove previous table */
		//lua_pop(L, 2);  /* remove table and value */        
		return;  /* return problematic part of the name */
	}
  } while (*e == '.');
  return;
}

/* }====================================================== */

static int create_guild (lua_State *L) {
	const char* name = luaL_checkstring(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);	
	std::string guild_name = name;
	OGSLib::Guild* guild = This->getGSL()->GetGuildManager()->CreateGuild(guild_name);
	if( guild )
	{
		This->get_guildbind()->bind(L,guild);
		return 1;
	}
	return 0;
}
static int find_guild (lua_State *L) {
	const char* name = luaL_checkstring(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	std::string guild_name = name;
	OGSLib::Guild* guild = This->getGSL()->GetGuildManager()->FindGuildByName(guild_name);
	if( guild )
	{
		This->get_guildbind()->bind(L,guild);
		return 1;
	}
	return 0;
}
static int find_map(lua_State *L){
	if( lua_gettop(L)<=0 || lua_isnil(L,1) ) return 0;
	const char* mapid = luaL_checkstring(L,1);
	std::string map_id;
	if( mapid )
	{
		map_id = mapid;
	}

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	OGSLib::PlayMap* map = This->getGSL()->GetMapManager()->FindMap(map_id);
	if( map )
	{
		This->get_mapbind()->bind(L,map);
		return 1;
	}
	return 0;
}
static int find_player(lua_State *L){
	if( lua_gettop(L)<=0 || lua_isnil(L,1) ) return 0;
	const char* name = luaL_checkstring(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	std::string guild_name = name;
	OGSLib::GameSession* session = This->getGSL()->FindSessionWithCharID(name);
	if( session && session->GetPlayer() )
	{
		This->get_playerbing()->bind(L,session->GetPlayer());
		return 1;
	}
	return 0;
}
static int find_player_by_seed(lua_State *L){
	if( lua_gettop(L)<=0 || lua_isnil(L,1) ) return 0;
	const char* name = luaL_checkstring(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	std::string guild_name = name;
	OGSLib::GameSession* session = This->getGSL()->FindSessionWithSeedID(name);
	if( session && session->GetPlayer() )
	{
		This->get_playerbing()->bind(L,session->GetPlayer());
		return 1;
	}
	return 0;
}
static int new_group(lua_State *L){
	std::string map_name = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	OGSLib::Group* g = This->getGSL()->GetGroupManager()->CreateGroup(map_name);
	if( g )
	{
		lua_pushinteger(L,g->ID());
		return 1;
	}
	return 0;
}
static int find_map_group(lua_State *L){
	std::string map_name = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	OGSLib::Group* g = This->getGSL()->GetGroupManager()->FindTaskGroup(map_name);
	if( g )
	{
		lua_pushinteger(L,g->ID());
		return 1;
	}
	return 0;
}
static int add_group_member(lua_State *L){
	int gid = luaL_checkint(L,1);
	const char* nick = luaL_checkstring(L,2);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	if (nick)
	{
		std::string strnick = nick;
		OGSLib::GameSession* session = This->getGSL()->FindSessionWithCharID(strnick);
		if( session && session->GetDomainGroup() )
		{
			session->GetDomainGroup()->EnterGroup(gid);
		}
	}
	return 0;
}
static int rem_group_member(lua_State *L){
	int gid = luaL_checkint(L,1);
	const char* nick = luaL_checkstring(L,2);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	if (nick)
	{
		std::string strnick = nick;
		OGSLib::GameSession* session = This->getGSL()->FindSessionWithCharID(strnick);
		if( session && session->GetDomainGroup() )
		{
			session->GetDomainGroup()->LeaveGroup();
		}
	}
	return 0;
}
static int set_group_name(lua_State* L)
{
	int gid = luaL_checkint(L,1);
	const char* name = luaL_checkstring(L,2);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	if( name )
	{
		std::string strname=name;
		OGSLib::Group* g=This->getGSL()->GetGroupManager()->FindGroup(gid);
		if( g )
		{
			g->SetName(strname);
		}
	}
	return 0;
}
static int lock_group(lua_State* L)
{
	int gid = luaL_checkint(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	OGSLib::Group* g=This->getGSL()->GetGroupManager()->FindGroup(gid);
	if( g )
	{
		g->mLock=true;
	}
	return 0;
}
static int unlock_group(lua_State* L)
{
	int gid = luaL_checkint(L,1);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	OGSLib::Group* g=This->getGSL()->GetGroupManager()->FindGroup(gid);
	if( g )
	{
		g->mLock=false;
	}
	return 0;
}
static int set_var(lua_State* L)
{
	const char* key = luaL_checkstring(L,1);
	const char* value = luaL_checkstring(L,2);
	std::string k = key;
	std::string v = value;

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	This->getGSL()->Var(k,v);
	return 0;
}
static int get_var(lua_State* L)
{
	const char* key = luaL_checkstring(L,1);
	std::string k = key;

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	lua_pushstring(L,This->getGSL()->Var(k).c_str() );
	return 1;
}
static int set_temp_var(lua_State* L)
{
	const char* key = luaL_checkstring(L,1);
	const char* value = luaL_checkstring(L,2);
	std::string k = key;
	std::string v = value;

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	This->getGSL()->TempVar(k,v);
	return 0;
}
static int get_temp_var(lua_State* L)
{
	const char* key = luaL_checkstring(L,1);
	std::string k = key;

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	lua_pushstring(L,This->getGSL()->TempVar(k).c_str() );
	return 1;
}
static int info(lua_State* L)
{
	int lv = luaL_checkinteger(L,1);
	int flags = luaL_checkinteger(L,2);
	const char* m = luaL_checkstring(L,3);
	//std::string msg(m);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	This->getGSL()->ServerAlertMessage(lv,flags,m);
	return 0;
}
static int play_effect(lua_State* L)
{
	int skill_type = luaL_checkinteger(L,1);
	int res_id = luaL_checkinteger(L,2);

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	This->getGSL()->ServerPlayEffect(skill_type,res_id);
	return 0;
}
static int get_kuafu_service(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->GetListener()->GetKuaFuService());
	return 1;
}
static int get_kuafu_state(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->GetKuafuState());
	return 1;
}
static int start_kuafu(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->StartKuafu();
	return 0;
}
static int stop_kuafu(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->StopKuafu();
	return 0;
}
static int load_csv_file(lua_State* L)
{
	const char* key = luaL_checkstring(L,1);
	std::string file = key;

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	size_t size;
	const char* data = This->getGSL()->GetListener()->LoadFile(file.c_str(),size);
	if( data && size > 0 )
	{
		std::string result = std::string(data,size);

		static char dest[1024*1024];
		dest[0]=0;
		static wchar_t temp[1024*1024];
		temp[0]=0;
		int wl = ::MultiByteToWideChar(CP_ACP,0,result.c_str(),result.length(),temp,1024*1024);
		if( wl > 0 )
		{
			int ml = ::WideCharToMultiByte(CP_UTF8,0,temp,wl,dest,1024*1024,0,false);
			dest[ml]=0;
		}
		result=dest;

		lua_pushstring(L,result.c_str());
		return 1;
	}
	return 0;
}
static int get_stronger_kuafu_guild(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g;
	This->getGSL()->GetStrongerKuafuGuild(g);
	lua_pushstring(L,g.c_str());
	return 1;
}
static int set_stronger_kuafu_guild(lua_State* L)
{
	const char* gu = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g(gu);
	This->getGSL()->SetStrongerKuafuGuild(g);
	return 0;
}
static int get_stronger_kuafu_area(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g;
	This->getGSL()->GetStrongerKuafuArea(g);
	lua_pushstring(L,g.c_str());
	return 1;
}
static int set_stronger_kuafu_area(lua_State* L)
{
	const char* gu = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g(gu);
	This->getGSL()->SetStrongerKuafuArea(g);
	return 0;
}
static int get_war_state(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->GetWarState());
	return 1;
}
static int start_war(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->StartWar();
	return 0;
}
static int PushServerStartDay(lua_State* L)
{
	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);
	This->getGSL()->SetServerDays();
	return 0;
}
static int PushMergeStartDay(lua_State* L)
{
	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);
	This->getGSL()->SetMergeDays();
	return 0;
}
static int PushActivityDay(lua_State* L)
{
	int begin = luaL_checkinteger(L, 1);//活动开始日期
	int end = luaL_checkinteger(L, 2);//活动结束日期
	int today = luaL_checkinteger(L, 3);//金条的日子
	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);
	This->getGSL()->SetAcitivityDays(begin,end,today);
	return 0;
}
static int stop_war(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->StopWar();
	return 0;
}
static int is_war(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->IsWar());
	return 1;
}
static int get_king_guild(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g;
	This->getGSL()->GetKingGuild(g);
	lua_pushstring(L,g.c_str());
	return 1;
}
static int get_prosperity(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->Getprosperity());
	return 1;
}
static int set_king_guild(lua_State* L)
{
	const char* gu = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string g(gu);
	This->getGSL()->SetKingGuild(g);
	return 0;
}
static int set_fly_node(lua_State* L)
{

	return 0;
}
static int server_exe(lua_State* L)
{
	const char* code = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->ServerExe(code);
	return 0;
}

static int server_gexe(lua_State* L)
{
	const char* code = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->ServerGuild(code);
	return 0;
}

static int get_exp_mul(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	lua_pushinteger(L,This->getGSL()->mExpMul);
	return 1;
}
static int set_exp_mul(lua_State* L)
{
	int em = luaL_checkinteger(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	if( em >= 1 && em < 10 )
	{
		This->getGSL()->mExpMul=em;
	}
	return 0;
}
static int get_server_name(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushstring(L,This->getGSL()->mServerName.c_str());
	return 1;
}
static int get_server_title(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushstring(L,This->getGSL()->mServerTitle.c_str());
	return 1;
}
static int get_server_home(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushstring(L,This->getGSL()->mServerHome.c_str());
	return 1;
}

static int get_server_id(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushinteger(L,This->getGSL()->mServerID);
	return 1;
}

static int post_mail_to_one(lua_State* L)
{
	std::string seedname = luaL_checkstring(L,1);
	std::string title = luaL_checkstring(L,2);
	std::string content = luaL_checkstring(L,3);
	std::map<int,int>items;
	for(int i=1;i<=10;i++)
	{
		if(lua_isnumber(L,i*2+2)&&lua_isnumber(L,i*2+3))
		{
			int itemID = luaL_checkinteger(L,i*2+2);
			int num = luaL_checkinteger(L,i*2+3);
			items[itemID] += num;
		}
		else
		{
			break;
		}
	}

	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);

	This->getGSL()->GetMailManager()->PostMailToOnePlayer(title,content,seedname,items);
	return 0;
}
static int server_post_mail(lua_State* L)
{
	std::string title = luaL_checkstring(L,1);
	std::string content = luaL_checkstring(L,2);
	std::map<int,int>items;
	for(int i=2;i<=20;i++)
	{
		if(lua_isnumber(L,i*2-1)&&lua_isnumber(L,i*2))
		{
			int itemID = luaL_checkinteger(L,i*2-1);
			int num = luaL_checkinteger(L,i*2);
			items[itemID] += num;
		}
		else
		{
			break;
		}
	}
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetMailManager()->PostMailToAllPlayers(title,content,items);
	return 1;
}

static int post_verify_mail(lua_State* L)
{
	std::string title = luaL_checkstring(L,1);
	std::string content = luaL_checkstring(L,2);
	std::map<int,int>items;
	for(int i=2;i<=20;i++)
	{
		if(lua_isnumber(L,i*2-1)&&lua_isnumber(L,i*2))
		{
			int itemID = luaL_checkinteger(L,i*2-1);
			int num = luaL_checkinteger(L,i*2);
			items[itemID] += num;
		}
		else
		{
			break;
		}
	}
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetMailManager()->PostMailToAllPlayers(title,content,items, 0,1);
	return 1;
}

static int get_server_platform(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushstring(L,This->getGSL()->mServerPlatform.c_str());
	return 1;
}
static int add_item_triger(lua_State* L)
{
	int type_id = luaL_checkinteger(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetItemDefineManager()->AddItemTriger(type_id);
	return 0;
}
static int add_item_alert(lua_State* L)
{
	int type_id = luaL_checkinteger(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetItemDefineManager()->AddItemAlert(type_id);
	return 0;
}
static int get_online_num(lua_State* L)
{
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	lua_pushnumber(L,static_cast<int>(This->getGSL()->GetSessionNumber()));
	return 1;
}
static int start_chargedart(lua_State* L)
{
	std::string charName = luaL_checkstring(L,1);
	int dartCar = luaL_checkinteger(L,2);
	int icon = luaL_checkinteger(L,3);
	int duration = luaL_checkinteger(L,4);
	int fightingForce = luaL_checkinteger(L,5);
	int totalAwards = luaL_checkinteger(L,6);
	int dartLevel = luaL_checkinteger(L,7);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetChargeDartManager()->StartChargeDart(charName,dartCar,icon,duration,fightingForce,totalAwards,dartLevel);
	return 0;
}
static int get_chargedart(lua_State* L)
{
	std::string charName = luaL_checkstring(L,1);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	OGSLib::ChargeDart* cd = This->getGSL()->GetChargeDartManager()->GetChargeDart(charName);
	if( cd && lua_checkstack(L,20) )
	{
		lua_pushinteger(L,cd->mDartCar);
		lua_pushinteger(L,cd->mIcon);
		lua_pushinteger(L,cd->mStartTime);
		lua_pushinteger(L,cd->mDuration);
		lua_pushinteger(L,cd->mFightingForce);
		lua_pushinteger(L,cd->mStolenTimes);
		lua_pushinteger(L,cd->mTotalAwards);
		lua_pushinteger(L,cd->mSurplusAwards);
		lua_pushinteger(L,cd->mState);
		lua_pushstring(L,cd->mRobDartCharNames.c_str());
		return 10;
	}
	return 0;
}
static int set_chargedart(lua_State* L)
{
	std::string charName = luaL_checkstring(L,1);
	int dartCar = luaL_checkinteger(L,2);
	int icon = luaL_checkinteger(L,3);
	double startTime = luaL_checknumber(L,4);
	int duration = luaL_checkinteger(L,5);
	int fightingForce = luaL_checkinteger(L,6);
	int	stolenTimes = luaL_checkinteger(L,7);
	int totalAwards = luaL_checkinteger(L,8);
	int surplusAwards = luaL_checkinteger(L,9);
	int state = luaL_checkinteger(L,10);
	std::string robDartCharNames = luaL_checkstring(L,11);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	This->getGSL()->GetChargeDartManager()->SetChargeDart(charName,dartCar,icon,startTime,duration,fightingForce,stolenTimes,totalAwards,surplusAwards,state,robDartCharNames);
	return 0;
}
static int server_exe_rand(lua_State* L)
{
	const char* name = luaL_checkstring(L,1);
	int num = luaL_checkint(L,2);
	int level = luaL_checkint(L,3);
	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);
	std::string name_str = This->getGSL()->ServerExeRand(name,num,level);
	lua_pushstring(L,name_str.c_str());
	return 1;
}
static int log_act(lua_State* L)
{
	const char* actname = luaL_checkstring(L, 1);
	int num = luaL_checkint(L, 2);
	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);

	return 0;
}
static int set_timer(lua_State* L)
{
	int i = luaL_checkinteger(L, 1);
	int esp = luaL_checkinteger(L, 2);

	lua_getglobal(L, "server");
	lua_getfield(L, -1, "This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L, -1);
	lua_pop(L, 2);

	This->getGSL()->GetAutoRunManager()->SetMapTimer(i, esp);
	return 0;
}

static const luaL_Reg server_funcs[] = {
	{"cjson", luaopen_cjson_safe},
	{"create_guild", create_guild},
	{"find_guild", find_guild},
	{"find_map", find_map},
	{"find_player", find_player},
	{"find_player_by_seed", find_player_by_seed},
	{"new_group", new_group},
	{"find_map_group", find_map_group},
	{"add_group_member", add_group_member},
	{"rem_group_member", rem_group_member},
	{"set_group_name", set_group_name},
	{"lock_group", lock_group},
	{"unlock_group", unlock_group},
	{"set_var", set_var},
	{"get_var", get_var},
	{"set_temp_var", set_temp_var},
	{"get_temp_var", get_temp_var},
	{"info",info},
	{"play_effect",play_effect},
	{"get_war_state",get_war_state},
	{"start_war",start_war},
	{"stop_war",stop_war},
	{"is_war",is_war},
	{"get_king_guild",get_king_guild},
	{"set_king_guild",set_king_guild},
	{"server_exe",server_exe},
	{"server_gexe",server_gexe},
	{"get_prosperity",get_prosperity},
	{"set_exp_mul",set_exp_mul},
	{"get_exp_mul",get_exp_mul},
	{"get_server_name",get_server_name},
	{"get_server_title",get_server_title},
	{"get_server_home",get_server_home},
	{"get_server_platform",get_server_platform},
	{"add_item_triger",add_item_triger},
	{"add_item_alert",add_item_alert},
	{"server_post_mail",server_post_mail},
	{"post_mail_to_one",post_mail_to_one},
	{"post_verify_mail",post_verify_mail},
	{"set_fly_node",set_fly_node},
	{"get_online_num",get_online_num},
	{"start_chargedart",start_chargedart},
	{"get_chargedart",get_chargedart},
	{"set_chargedart",set_chargedart},
	{"get_kuafu_service",get_kuafu_service},
	{"get_kuafu_state",get_kuafu_state},
	{"start_kuafu",start_kuafu},
	{"stop_kuafu",stop_kuafu},
	{"load_csv_file",load_csv_file},
	{"get_stronger_kuafu_guild",get_stronger_kuafu_guild},
	{"set_stronger_kuafu_guild",set_stronger_kuafu_guild},
	{"get_stronger_kuafu_area",get_stronger_kuafu_area},
	{"set_stronger_kuafu_area",set_stronger_kuafu_area},
	{"server_exe_rand",server_exe_rand},
	{"log_act",log_act},
	{"set_timer",set_timer},
	{"PushServerStartDay",PushServerStartDay},
	{"PushMergeStartDay",PushMergeStartDay},
	{"PushActivityDay",PushActivityDay},
	{"get_server_id", get_server_id},
	{NULL, NULL}
};


LUALIB_API int luaopen_server (lua_State *L) {
  luaL_register(L, "server", server_funcs);
  return 1;
}

typedef struct MyLoadF {
	const char *buf;
	size_t pos;
	size_t size;
} LoadF;
static const char *my_getF (lua_State *L, void *ud, size_t *size) {
	MyLoadF *lf = (MyLoadF *)ud;
	(void)L;
	*size = lf->size-lf->pos;
	size_t op = lf->pos;
	lf->pos+=*size;
	return lf->buf+op;
}
static int load_file(lua_State* L,const char* name) {
	//const char *filename = findfile(L, name, "cpath");
	//if (filename == NULL) return 1;  /* library not found in this path */
	char temp[256];
	strncpy_s(temp,255,name,255);
	temp[255]=0;
	for(size_t i = 0;i<strlen(temp);i++)
	{
		if( temp[i] == '.' )
		{
			temp[i] = '/';
		}
	}

	lua_getglobal(L,"server");
	lua_getfield(L,-1,"This");
	OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
	lua_pop(L,2);

	MyLoadF lf;
	char funcname[256];
	_snprintf_s(funcname,255,255,"script/%s."SCRIPT_EXP,temp);
	funcname[255]=0;
	lf.buf = This->getGSL()->GetListener()->LoadFile(funcname,lf.size);
	lf.pos = 0;
	if( lf.buf==0 )
	{
		_snprintf_s(funcname,255,255,"script/%s/init."SCRIPT_EXP,name);
		funcname[255]=0;
		lf.buf = This->getGSL()->GetListener()->LoadFile(funcname,lf.size);
		lf.pos = 0;
	}
	int status=LUA_ERRERR;
	if( lf.buf && lf.size > 0 )
	{
		//char * buf = new char[lf.size];
		//memcpy(buf,lf.buf,lf.size);
		//lf.buf = buf;
		status = lua_load(L, my_getF, &lf, name);
		//delete []buf;
	}
	else
	{
		printf("load script file %s error\n",name);
	}
	return status;
}
static int loader_Lc (lua_State *L) {
	const char *name = luaL_checkstring(L, 1);
	int status = load_file(L,name);
	if( status == 0 )
	{
		return 1;
	}
	report(L,status,"");
	return 0;
}
static int dofile(lua_State *L, const char *name) {
  int status = luaL_loadfile(L, name) || docall(L, 0, 1);
  return report(L, status,"");
}
static int dofile2(lua_State *L, const char *name) {
  int status = load_file(L, name) || docall(L, 0, 1);
  return report(L, status,"");
}

namespace OGSLib
{
	lua_State* ScriptLua::load_script(const char* path)
	{
		lua_State* L;
		L = lua_open();
		lua_gc(L, LUA_GCSTOP, 0);
		luaL_openlibs(L);
		luaopen_server(L);
		m_npcbind->init(L);
		m_playerbind->init(L);
		m_monsterbind->init(L);
		m_guildbind->init(L);
		m_mapbind->init(L);
		m_itembind->init(L);
		m_slavebind->init(L);
		lua_gc(L, LUA_GCRESTART, 0);

		lua_getglobal(L,"server");
		ScriptLua** a = (ScriptLua**)lua_newuserdata(L, sizeof(ScriptLua*));
		*a = this;
		lua_setfield(L,-2,"This");
		lua_pop(L,1);

		lua_getglobal(L,"package");
		lua_pushstring(L,"");
		lua_setfield(L,-2,"path");
		lua_pop(L,1);

		lua_getfield(L, LUA_GLOBALSINDEX, "package");
		lua_getfield(L, -1, "loaders");
		lua_remove(L, -2);

		lua_pushinteger(L, 1);
		lua_pushcfunction(L, loader_Lc);
		lua_rawset(L, -3);

		lua_pop(L, 1);

		if( dofile2(L,"init") != 0 )
		{
			lua_close(L);
			L = 0;
		}
		return L;
	}
	ScriptLua::ScriptLua(GameServerLib* gsl)
	{
		m_l = 0;
		m_game_server_lib = gsl;
		m_npcbind = new ScriptLuaNpcBind;
		m_playerbind = new ScriptLuaPlayerBind;
		m_monsterbind = new ScriptLuaMonsterBind;
		m_guildbind = new ScriptLuaGuildBind;
		m_mapbind = new ScriptLuaMapBind;
		m_itembind = new ScriptLuaItemBind;
		m_slavebind = new ScriptLuaSlaveBind;

		mNpcPlayerCount=0;
		mRetNpcPlayerCount=0;
		mItemPlayerCount=0;
		mAs2LuaPlayerCount=0;
		mAs2LuaPlayer2Count=0;
		mPlayerIntCount=0;
		mMonsterPlayerCount=0;
		mPlayerCongziCount=0;
		mPlayerStringCount=0;
		mOnlyMapCount=0;
		mOnlyPlayerCount=0;
		mOnlyPlayerParamCount = 0;
		mMapPlayerCount=0;
	}

	ScriptLua::~ScriptLua(void)
	{
		delete m_npcbind;m_npcbind=0;
		delete m_playerbind;m_playerbind=0;
		delete m_monsterbind;m_monsterbind=0;
		delete m_guildbind;m_guildbind=0;
		delete m_mapbind;m_mapbind=0;
		delete m_itembind;m_itembind=0;
		delete m_slavebind;m_slavebind = 0;
	}
	void ScriptLua::set_state(lua_State* l)
	{
		if( ! l ) return;
		lua_State* ol = m_l;
		m_l = l;
		if( ol )
		{
			//printf("lua memory used:%dmb\n",(lua_gc(ol,LUA_GCCOUNT,0)+lua_gc(ol,LUA_GCCOUNTB,0)/1024)/1024);
			lua_close(ol);
		}
		DoCall("OnInit");
	}
	size_t ScriptLua::mem_size()
	{
		if( m_l )
		{
			return (lua_gc(m_l,LUA_GCCOUNT,0)*1024+lua_gc(m_l,LUA_GCCOUNTB,0));
		}
		return 0;
	}
	bool ScriptLua::init(const char* path)
	{
		strncpy_s(m_script_path,1023,path,1023);
		m_script_path[1023]=0;
		set_state(load_script(m_script_path));
		return true;
	}
	void ScriptLua::dest()
	{
		printf("lua memory used:%dmb\n",(lua_gc(m_l,LUA_GCCOUNT,0)+lua_gc(m_l,LUA_GCCOUNTB,0)/1024)/1024);
		lua_close(m_l);
		m_l = 0;
	}
	void ScriptLua::reload()
	{
		set_state(load_script(m_script_path));
	}
	void ScriptLua::DoCall(const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				report(m_l, lua_pcall(m_l,0,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::NpcPlayer(PlayNpc* npc,DomainPlay* player,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_npcbind->bind(m_l,npc);
				m_playerbind->bind(m_l,player);
				mNpcPlayerCount++;
				if( mNpcPlayerCount > 10 ){printf("npcplayer,%s",function);}
				report(m_l, lua_pcall(m_l,2,0,0),function);
				mNpcPlayerCount--;
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	int ScriptLua::RetNpcPlayer(PlayNpc* npc,DomainPlay* player,const char* function)
	{
		int ret = 1;
		if( ! m_l ) return ret;
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_npcbind->bind(m_l,npc);
				m_playerbind->bind(m_l,player);
				mRetNpcPlayerCount++;
				if( mRetNpcPlayerCount > 10 ){printf("RetNpcPlayer,%s",function);}
				report(m_l, lua_pcall(m_l,2,1,0),function);
				mRetNpcPlayerCount--;
				if( lua_isnumber(m_l,-1) )
				{
					ret = lua_tointeger(m_l,-1);
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
		return ret;
	}

	int ScriptLua::PlayerInt(DomainPlay* player,int param1,int param2,int param3,int param4,const char* function)
	{
		int ret=0;
		if( ! m_l ) return ret;
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushinteger(m_l,param1);
				lua_pushinteger(m_l,param2);
				lua_pushinteger(m_l,param3);
				lua_pushinteger(m_l,param4);
				mPlayerIntCount++;
				if(mPlayerIntCount > 10){printf("PlayerInt,%s",function);}
				report(m_l, lua_pcall(m_l,5,1,0),function);
				mPlayerIntCount--;
				if( lua_isnumber(m_l,-1) )
				{
					ret = lua_tointeger(m_l,-1);
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
		return ret;
	}

	int ScriptLua::PlayerInt(DomainPlay* player,std::vector<int>& params,const char* function)
	{
		int ret=0;
		if( ! m_l ) return ret;
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				for (size_t i = 0; i < params.size(); ++ i)
				{
					lua_pushinteger(m_l,params[i]);
				}
				mPlayerIntCount++;
				if(mPlayerIntCount > 10){printf("PlayerInt,%s",function);}
				report(m_l, lua_pcall(m_l,5,1,0),function);
				mPlayerIntCount--;
				if( lua_isnumber(m_l,-1) )
				{
					ret = lua_tointeger(m_l,-1);
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
		return ret;
	}

	std::string ScriptLua::PlayerGetName(DomainPlay* player,const char* item_name,const char* function)
	{
		std::string ret = "";
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushstring(m_l,item_name);
				report(m_l, lua_pcall(m_l,2,1,0),function);
				if( lua_isstring(m_l,-1) )
				{
					ret = lua_tostring(m_l,-1);
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
		return ret;
	}

	int ScriptLua::ItemPlayer(DomainPlay* player,const char* item_name,int item_id,const char* function,int num,int pos,const char* seed_id /*= NULL*/)
	{
		int ret = 1;
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushstring(m_l,item_name);
				lua_pushinteger(m_l,item_id);
				lua_pushinteger(m_l,num);
				lua_pushinteger(m_l,pos);
				if (seed_id != NULL)
					lua_pushstring(m_l, seed_id);
				else
					lua_pushstring(m_l, "");
				mItemPlayerCount++;
				if(mItemPlayerCount > 10){printf("ItemPlayer,%s",function);}
				report(m_l, lua_pcall(m_l,6,1,0),function);
				mItemPlayerCount--;
				if( lua_isnumber(m_l,-1) )
				{
					ret = lua_tointeger(m_l,-1);
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
		return ret;
	}

	int ScriptLua::PlayerString(DomainPlay* player, const char* function, const char* param)
	{
		int ret = 1;
		try
		{
			luaL_findfunction(m_l, LUA_GLOBALSINDEX, function);
			if (lua_isfunction(m_l, -1))
			{
				m_playerbind->bind(m_l, player);
				lua_pushstring(m_l, param);
				mPlayerStringCount++;
				if (mPlayerStringCount > 10)printf("PlayerString,%s", function);
				report(m_l, lua_pcall(m_l, 2, 1, 0), function);
				if (lua_isnumber(m_l, -1))
				{
					ret = lua_tointeger(m_l, -1);
				}
				mPlayerStringCount--;
			}
			else
			{
				lua_pop(m_l, 1);
			}
		}
		catch (...)
		{
		}
		return ret;
	}
	int ScriptLua::GetServerStartDay(const char* function)
	{
		int ret = 0;
		try
		{
			luaL_findfunction(m_l, LUA_GLOBALSINDEX, function);
			if (lua_isfunction(m_l, -1))
			{
				mItemPlayerCount++;
				if (mItemPlayerCount > 10){ printf("GetServerStartDay,%s", function); }
				report(m_l, lua_pcall(m_l, 0, 1, 0), function);
				mItemPlayerCount--;
				if (lua_isnumber(m_l, -1))
				{
					ret = lua_tointeger(m_l, -1);
				}
			}
			else
			{
				lua_pop(m_l, 1);
			}
		}
		catch (...)
		{
		}
		return ret;
	}
	void ScriptLua::PlayerLuaFunction(DomainPlay* player,const char* function,const char* param,const char* data)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushstring(m_l,param);
				lua_pushstring(m_l,data);
				report(m_l, lua_pcall(m_l,3,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::OnlyPlayer(DomainPlay* player,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				mOnlyPlayerCount++;
				if(mOnlyPlayerCount > 10){printf("OnlyPlayer,%s\n",function);}
				int result = report(m_l, lua_pcall(m_l,1,0,0),function);
				mOnlyPlayerCount--;
				if (result == 4)///出现异常之后重读脚本
				{
					printf("OnlyPlayer exception then reload script\n");
					reload();
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::OnlyPlayer(DomainPlay* player,int param1,int param2,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushinteger(m_l,param1);
				lua_pushinteger(m_l,param2);
				mOnlyPlayerParamCount++;
				if (mOnlyPlayerParamCount > 10){ printf("OnlyPlayerParam,%s\n", function); }
				int result = report(m_l, lua_pcall(m_l,3,0,0),function);
				mOnlyPlayerParamCount--;
				if (result == 4)///出现异常之后重读脚本
				{
					printf("OnlyPlayerParam exception then reload script\n"); 
					reload();
				}
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::PlayerPlayer(DomainPlay* player,DomainPlay* des,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				m_playerbind->bind(m_l,des);
				report(m_l, lua_pcall(m_l,2,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::PlayerMonster(DomainPlay* player,PlayMonster* monster,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				m_monsterbind->bind(m_l,monster);
				report(m_l, lua_pcall(m_l,2,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::MonsterPlayer(PlayMonster* monster,DomainPlay* player,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_monsterbind->bind(m_l,monster);
				m_playerbind->bind(m_l,player);
				mMonsterPlayerCount++;
				if(mMonsterPlayerCount > 10){printf("MonsterPlayer,%s",function);}
				report(m_l, lua_pcall(m_l,2,0,0),function);
				mMonsterPlayerCount--;
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::OnlyMonster(PlayMonster* monster,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_monsterbind->bind(m_l,monster);
				report(m_l, lua_pcall(m_l,1,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::OnlyGuild(Guild* guild,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_guildbind->bind(m_l,guild);
				report(m_l, lua_pcall(m_l,1,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}

	void ScriptLua::OnlySlave(PlaySlave * slave, const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_slavebind->bind(m_l,slave);
				report(m_l, lua_pcall(m_l,1,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}

	void ScriptLua::PlayerCongzi(DomainPlay* player,int congzi,int congzihou,int congziaccu,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_playerbind->bind(m_l,player);
				lua_pushinteger(m_l,congzi);
				lua_pushinteger(m_l,congzihou);
				lua_pushinteger(m_l,congziaccu);
				mPlayerCongziCount++;
				if(mPlayerCongziCount > 10){printf("PlayerCongzi,%s",function);}
				report(m_l, lua_pcall(m_l,4,0,0),function);
				mPlayerCongziCount--;
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::MapPlayer(PlayMap* map,DomainPlay* player,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_mapbind->bind(m_l,map);
				m_playerbind->bind(m_l,player);
				mMapPlayerCount++;
				if(mMapPlayerCount > 10){printf("MapPlayer,%s",function);}
				report(m_l, lua_pcall(m_l,2,0,0),function);
				mMapPlayerCount--;
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::OnlyMap(PlayMap* map,const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				m_mapbind->bind(m_l,map);
				mOnlyMapCount++;
				if(mOnlyMapCount > 10){printf("OnlyMap,%s",function);}
				report(m_l, lua_pcall(m_l,1,0,0),function);
				mOnlyMapCount--;
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
	void ScriptLua::SystemScript(const char* function)
	{
		try
		{
			luaL_findfunction(m_l,LUA_GLOBALSINDEX,function);
			if( lua_isfunction(m_l,-1) )
			{
				report(m_l, lua_pcall(m_l,0,0,0),function);
			}
			else
			{
				lua_pop(m_l,1);
			}
		}
		catch(...)
		{
		}
	}
}