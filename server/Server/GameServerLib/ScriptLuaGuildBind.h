#pragma once

namespace OGSLib
{
	class Guild;
	class ScriptLuaGuildBind
	{
	public:
		ScriptLuaGuildBind(void);
		virtual ~ScriptLuaGuildBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,Guild* guild);
	};
}