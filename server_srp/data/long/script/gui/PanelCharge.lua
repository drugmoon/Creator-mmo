module(..., package.seeall)
local Data = {
	["ios"] = {
		{ id="100006",  num = 6, 		vcoin = 600,		res="icon_01",			},
		{ id="100030",  num = 30, 	vcoin = 3000,	res="icon_02",			},
		{ id="100050",  num = 50, 	vcoin = 5000,	res="icon_03",			},
		{ id="100098",  num = 98, 	vcoin = 9800,	res="icon_04",			},
		{ id="100198",  num = 198, 	vcoin = 19800,	res="icon_05",	},
		{ id="100328",  num = 328, 	vcoin = 32800,	res="icon_06",	},
		{ id="100648",  num = 648, 	vcoin = 64800,	res="icon_07",		},
		{ id="101998",  num = 1998, 	vcoin = 199800,	res="icon_08",		},
	},
	["android"] = {
		{ id="100006",  num = 6, 		vcoin = 600,		res="icon_01",			},
		{ id="100030",  num = 30, 	vcoin = 3000,	res="icon_02",			},
		{ id="100050",  num = 50, 	vcoin = 5000,	res="icon_03",			},
		{ id="100098",  num = 98, 	vcoin = 9800,	res="icon_04",			},
		{ id="100198",  num = 198, 	vcoin = 19800,	res="icon_05",	},
		{ id="100328",  num = 328, 	vcoin = 32800,	res="icon_06",	},
		{ id="100648",  num = 648, 	vcoin = 64800,	res="icon_07",		},
		{ id="101000",  num = 1000, 	vcoin = 100000,	res="icon_08",		},
		-- { id="102000",  num = 2000, 	vcoin = 200000,	res="icon_08",		},
		-- { id="105000",  num = 5000, 	vcoin = 500000,	res="icon_09",		},
		--{ num = 5000, 	vcoin = 500000,	res="icon_09",		},
	},
	--google渠道专用
	[1001] = {
		{ id="100006",  num = 1, 		vcoin = 100,		res="icon_01",			},
		{ id="100030",  num = 10, 	vcoin = 1000,	res="icon_02",			},
		{ id="100050",  num = 50, 	vcoin = 5000,	res="icon_03",			},
	}
}

function onPanelData(player, commondData)
	local data =util.decode(commondData);
	if data.actionid == "fresh" then
		freshPanel(player, data.sys, data.platformId)
	elseif data.actionid == "charge" then
		--onCharge(player,data.idx)
	end
end
ckpanel.add_listener("gui.PanelCharge.onPanelData",gui.PanelCharge.onPanelData)

function freshPanel(player, sys, platformId)
	local result = {}
	if sys ~= "ios" then sys = "android" end
	local chargeData = Data[sys];
	if platformId == 1001 then
		chargeData = Data[platformId];
	end

   	result.cmd = "fresh"
    result.data = chargeData
   	result.vipLv = util.ppn(player, const.PP_VIP_LEVEL)
   	result.vipExp = util.ppn(player, const.PP_VIP_EXP)
   	result.vipMax = gui.PanelVip.getNextMaxCharge(player)
   	if result.vipLv < gui.PanelVip.getMaxLevel(player) then
   		result.chargeInfo = string.format("只要再充值%d元，就可以成为VIP%d",math.ceil(result.vipMax/100 - result.vipExp/100),result.vipLv+1)
   	else
		result.chargeInfo = "VIP等级已达最高级"
   	end
   	result.limit6Charge = util.ppn(player, const.PP_CHARGE6_LIMIT)>0
   	player:push_lua_table("PanelCharge",util.encode(result));
end

function onCharge( player, idx)

end

function onPlayCz(player)
  	player:push_lua_table("playcz",util.encode({actionid = "playcz",resid = 300027,jpg = "false"}))
end