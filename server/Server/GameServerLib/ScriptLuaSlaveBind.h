#pragma once

namespace OGSLib
{
	class PlaySlave;
	class ScriptLuaSlaveBind
	{
	public:
		ScriptLuaSlaveBind(void);
		virtual ~ScriptLuaSlaveBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,PlaySlave* slave);
	};
}