#pragma once
namespace OGSLib
{
	class PlayNpc;
	class ScriptLuaNpcBind
	{
	public:
		ScriptLuaNpcBind(void);
		virtual ~ScriptLuaNpcBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,PlayNpc* npc);
	};
}