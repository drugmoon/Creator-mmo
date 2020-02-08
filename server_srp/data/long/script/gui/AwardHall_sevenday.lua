module(... ,package.seeall)

--------------------------------------------------------------充值有礼------------------------------------------------------------------
local rankAwards = {
	[1]={
		svr="AwardHall_sevenday_name_1",
		seedsvr="AwardHall_sevenday_seedname_1",
		awards={
			{id = 28000012,	num = 1, bind = 1},
			{id = 23150002,	num = 10, bind = 1},
			{id = 23060003,	num = 5, bind = 1},
		}
	},
	[2]={
		svr="AwardHall_sevenday_name_2",
		seedsvr="AwardHall_sevenday_seedname_2",
		awards={
			{id = 28000013,	num = 1, bind = 1},
			{id = 23150002,	num = 8, bind = 1},
			{id = 23060003,	num = 4, bind = 1},
		}
	},
	[3]={
		svr="AwardHall_sevenday_name_3",
		seedsvr="AwardHall_sevenday_seedname_3",
		awards={
			{id = 28000013,	num = 1, bind = 1},
			{id = 23150002,	num = 6, bind = 1},
			{id = 23060003,	num = 3, bind = 1},
		}
	},
	[4]={
		svr="AwardHall_sevenday_name_4",
		seedsvr="AwardHall_sevenday_seedname_4",
		awards={
			{id = 28000014,	num = 1, bind = 1},
			{id = 23150002,	num = 4, bind = 1},
			{id = 23060003,	num = 2, bind = 1},
		}
	},
	[5]={
		svr="AwardHall_sevenday_name_5",
		seedsvr="AwardHall_sevenday_seedname_5",
		awards={
			{id = 28000014,	num = 1, bind = 1},
			{id = 23150002,	num = 2, bind = 1},
			{id = 23060003,	num = 1, bind = 1},
		}
	},
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if data.actionid == "updataSevenDay" then
		updataSevenDay(player)
	end
end
ckpanel.add_listener("gui.AwardHall_sevenday.handlePanelData",gui.AwardHall_sevenday.handlePanelData)

function updataSevenDay(player)
	local result = {
		cmd="updataSevenDay",
		myRank=0,
		curXiaoFei=util.ppn(player, const.PP_SEVENDAY_CHONGZHI_NUM),
		awards=util.clone(rankAwards)
	}
	for i=1,#result.awards do
		local itemData = result.awards[i]
		itemData.name=server.get_var(itemData.svr)
		if itemData.name==player:get_name() then
			result.myRank=i
		end
	end

	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	result.time=(7-server_start_day()-1)*24*3600+(24-H)*3600-M*60-S
	player:push_lua_table("PanelRewardHall",util.encode(result))
	gui.moduleRedPoint.checkSevenDay(player)
end

--计算充值排名前五的
function setRechargeRank(player)
	local curcz = util.ppn(player, const.PP_SEVENDAY_CHONGZHI_NUM)
	if curcz<200000 then return end--最低20万上榜
	local rankData={
		{name=server.get_var("AwardHall_sevenday_name_1"),seedname=server.get_var("AwardHall_sevenday_seedname_1"),czNum=util.svn("AwardHall_sevenday_cz_1")},--1
		{name=server.get_var("AwardHall_sevenday_name_2"),seedname=server.get_var("AwardHall_sevenday_seedname_2"),czNum=util.svn("AwardHall_sevenday_cz_2")},--2
		{name=server.get_var("AwardHall_sevenday_name_3"),seedname=server.get_var("AwardHall_sevenday_seedname_3"),czNum=util.svn("AwardHall_sevenday_cz_3")},--3
		{name=server.get_var("AwardHall_sevenday_name_4"),seedname=server.get_var("AwardHall_sevenday_seedname_4"),czNum=util.svn("AwardHall_sevenday_cz_4")},--4
		{name=server.get_var("AwardHall_sevenday_name_5"),seedname=server.get_var("AwardHall_sevenday_seedname_5"),czNum=util.svn("AwardHall_sevenday_cz_5")},--5
	}
	local tempTable = util.clone(rankData)
	local myIndex=0
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.seedname==player:get_seed_name() then
			table.remove(tempTable,i)
			myIndex=i
			break
		end
	end
	for j=1,5 do
		if not tempTable[j] or curcz>tempTable[j].czNum then
			table.insert(tempTable,j,{name=player:get_name(),seedname=player:get_seed_name(),czNum=curcz})
			if myIndex~=j then
				server.info(10010,0,"恭喜<font color='#00ccff'>"..player:get_name().."</font>成为七天狂欢<font color='#ff0000'>第"..j.."名</font>")
			end
			break
		end
	end
	for n=1,5 do
		if tempTable[n] then
			server.set_var("AwardHall_sevenday_name_"..n,tempTable[n].name)
			server.set_var("AwardHall_sevenday_seedname_"..n,tempTable[n].seedname)
			server.set_var("AwardHall_sevenday_cz_"..n,tempTable[n].czNum)
		else
			server.set_var("AwardHall_sevenday_name_"..n,"")
			server.set_var("AwardHall_sevenday_seedname_"..n,"")
			server.set_var("AwardHall_sevenday_cz_"..n,0)
		end
	end
end
login.add_login_listener(setRechargeRank)
function sendRankAwards()
	local serverDay = server_start_day()
	if serverDay==7 and server.get_var("AwardHall_sevenday_mail")~="1" then
		for i=1,#rankAwards do
			local itemData = rankAwards[i]
			local seedname =server.get_var(itemData.seedsvr)
			local player = server.find_player_by_seed(seedname)
			local t = {}
			for j=1,#itemData.awards do
				table.insert(t,checknumber(itemData.awards[j].id))
				table.insert(t,checknumber(itemData.awards[j].num))
			end
			if player then
				player:post_mail("七日狂欢", "您在开服活动-七日狂欢中名列前茅\n获得排名第"..i.."名奖励",unpack(t))
			elseif seedname then
				server.post_mail_to_one(seedname,"七日狂欢","您在开服活动-七日狂欢中名列前茅\n获得排名第"..i.."名奖励",unpack(t))
			end
		end
		server.set_var("AwardHall_sevenday_mail","1")
	end
end

function checkSevenDay( player,num )
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local serverDay = server_start_day()+1--server_start_day()开区天数实在0点跟新的
	if H==0 and M==0 and S<60 then
		serverDay=serverDay-1
	end
	if serverDay>7 then return end--超过活动期限
	player:set_param(const.PP_SEVENDAY_CHONGZHI_NUM,util.ppn(player, const.PP_SEVENDAY_CHONGZHI_NUM)+num)
	setRechargeRank(player)
end

--红点检测
function checkRedPoint(player)
	return false
end









