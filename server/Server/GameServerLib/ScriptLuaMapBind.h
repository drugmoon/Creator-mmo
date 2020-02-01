#pragma once
namespace OGSLib
{
	class PlayMap;
	class ScriptLuaMapBind
	{
	public:
		ScriptLuaMapBind(void);
		virtual ~ScriptLuaMapBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,PlayMap* npc);
	};
}
