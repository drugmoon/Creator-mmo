module(..., package.seeall)

function onPlayerEnter(map,player)

end

function onPlayerLeave(map,player)
	
end
function onTimer1(map)
	map:map_exe("go.home");
end