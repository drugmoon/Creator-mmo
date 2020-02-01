#pragma once

namespace OGSLib
{
	class PlayMonster;
	class ScriptLuaMonsterBind
	{
	public:
		ScriptLuaMonsterBind(void);
		virtual ~ScriptLuaMonsterBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,PlayMonster* monster);
	};
}