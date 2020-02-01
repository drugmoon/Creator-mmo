#pragma once

namespace OGSLib
{
	class SubItem;
	class ScriptLuaItemBind
	{
	public:
		ScriptLuaItemBind(void);
		virtual ~ScriptLuaItemBind(void);
		void init(lua_State* l);
		void bind(lua_State* l,SubItem* item);
	};
}