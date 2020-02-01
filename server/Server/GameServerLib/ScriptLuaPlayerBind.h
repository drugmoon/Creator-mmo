#pragma once

namespace OGSLib
{
	class DomainPlay;
	class ScriptLuaPlayerBind
	{
	public:
		ScriptLuaPlayerBind(void);
		virtual ~ScriptLuaPlayerBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,DomainPlay* player);
	};
}