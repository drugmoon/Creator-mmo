module(..., package.seeall)

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if data.actionid == "dissolveGroup" then--解散队伍
		player:group_exe("gui.PanelGroup.dissolveGroup")
	end
end

function dissolveGroup(player)
	player:push_lua_table("dissolveGroup",util.encode({}))
end

ckpanel.add_listener("gui.PanelGroup.handlePanelData",gui.PanelGroup.handlePanelData)