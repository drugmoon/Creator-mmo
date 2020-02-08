module(..., package.seeall)
--[[
	CUSTOMSPASS.conf.con 是一个encode({...}),stars:[123],isfirst= true/false,gotaward = true/false,passtimes=0
]]
local bind = {
	[1] = '绑定',
	[2] = '非绑定',
}
--#编号,天关章数,天关关数,地图ID,进入等级,挑战次数,门票,门票数量,奖励物品,奖励物品数量,奖励物品绑定状态,首通额外奖励,首通额外物品数量,首通物品绑定状态,推荐战力,掉落展示物品,关卡图片,关联系统提示,通关条件图片,通关条件描述
local keys = {"id","chapter","section","mapId","limitlv","challengeTimes","ticketId","ticket","awards","awardNum","awardBind","firstAwards","firstAwardNum","firstBind","power","dropout","chapterImage","achieve","passCondition","passDescription"}
local types= {"int","int","int","string","int","int","int","int","int[]","int[]","int[]","int[]","int[]","int[]","int","int[]","string","string","string","string"}
local temp = tab.parseCsvFile(server.load_csv_file("breakupConfig.csv"),keys,types)
breakupConfig={}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(breakupConfig,v)
	end
end

--#编号	天关章数	星级数量	奖励	奖励数量	奖励绑定状态
keys = {"id","chapter","star","awards","awardNum","bind"}
types = {"int","int","int","int[]","int[]","int[]"}
temp =tab.parseCsvFile(server.load_csv_file("breakupStar.csv"),keys,types)
breakupStar = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(breakupStar,v)
	end
end
table.sort(breakupStar,function(a,b)
	if a.chapter == b.chapter then
		return a.star<b.star
	else
		return a.chapter<b.chapter
	end
end)

CUSTOMSPASS = {};

Config = {}--没有奖励等信息
for i,v in pairs(breakupConfig) do
	local chapter = v.chapter
	local section = v.section
	local name = section%2 == 1 and "normal" or (section<6 and "outstand" or "boss")
	if not CUSTOMSPASS[chapter] then CUSTOMSPASS[chapter] = {} end
	if not CUSTOMSPASS[chapter].conf then CUSTOMSPASS[chapter].conf = {} end
	CUSTOMSPASS[chapter].maxStar = 18
	CUSTOMSPASS[chapter].boxCon = const["AWARD_BOX_CON"..chapter]
	CUSTOMSPASS[chapter].firstCon = const["PP_BREAKUP_FIRST"..chapter]
	v.name = name
	CUSTOMSPASS[chapter].conf[section] = v
	CUSTOMSPASS[chapter].conf[section].con = const[string.format("CUSTOM_PASS_%d_%d",chapter,section)]

	if not Config[chapter] then Config[chapter] = {} end
	if not Config[chapter].conf then Config[chapter].conf = {} end
	Config[chapter].maxStar = 18
	Config[chapter].boxCon = const["AWARD_BOX_CON"..chapter]
	Config[chapter].firstCon = const["PP_BREAKUP_FIRST"..chapter]
	Config[chapter].conf[section] = {
		chapter = chapter,
		section = section,
		challengeTimes = v.challengeTimes,
		con = const[string.format("CUSTOM_PASS_%d_%d",chapter,section)],
		limitlv = v.limitlv,
		ticket = v.ticket,
		-- ticketId = v.ticketId,
		power = v.power,
		achieve = v.achieve,
		name = name ,
	}
end

function getSectionConfig(chapter,section)
	if CUSTOMSPASS[chapter] and CUSTOMSPASS[chapter].conf[section] then
		return CUSTOMSPASS[chapter].conf[section]
	end
end

function getSectionAward(chapter,section)
	local config,award = getSectionConfig(chapter,section),{}
	if config then
		for i=1,#config.awards do
			table.insert(award,{
				id = config.awards[i],
				num = config.awardNum[i],
				bind = 2-config.awardBind[i],
			})
		end
	end
	return award
end

function getSectionFirstAward(chapter,section)
	local config,award = getSectionConfig(chapter,section),{}
	if config then
		for i=1,#config.firstAwards do
			table.insert(award,{
				id = config.firstAwards[i],
				num = config.firstAwardNum[i],
				bind = 2-config.firstBind[i],
			})
		end
	end
	return award
end

function getSectionDropout(chapter,section)
	local config,award = getSectionConfig(chapter,section),{}
	if config then
		for i=1,#config.dropout do
			if config.dropout[i]>0 then
				table.insert(award,{
					id = config.dropout[i],
					num = 1,
					bind = 0,
				})
			end
		end
	end
	return award
end
keys = {"id","chapter","section","bossid","bossname","monid","monname","monnum"}
types = {"int","int","int","int","string","int[]","int[]","int[]"}
temp = tab.parseCsvFile(server.load_csv_file("breakupMonster.csv"),keys,types)
monConf = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(monConf,v)
	end
end
function getMonster(chapter,section)
	for k,v in pairs(monConf) do
		if v.chapter == chapter and v.section == section then
			return v
		end
	end
end

function getChapterBox(chapter)
	local chapterbox = {}
	for k,v in pairs(breakupStar) do
		if v.chapter == chapter then
			table.insert(chapterbox,v)
		end
	end
	return chapterbox
end
function getNeedStar(index,chapter)
	local chapterbox = getChapterBox(chapter)
	if chapterbox[index] then return chapterbox[index].star end
end

function getChapterBoxAward(index,chapter)
	local chapterbox = getChapterBox(chapter)
	local award = {}
	if chapterbox[index] then
		for i=1,#chapterbox[index].awards do
			table.insert(award,{
				id = chapterbox[index].awards[i],
				num = chapterbox[index].awardNum[i],
				bind = chapterbox[index].bind[i],
			})
		end
	end
	return award
end

--每关奖励
awardStar = {
	["normal"] = {
		stars = {5,3.5,2},
		countDown = 30,--结束倒计时提示
	},
	["outstand"] = {
		stars = {7,4.5,2},
		countDown = 60,--结束倒计时提示
	},
	["boss"] = {
		stars = {10,6.5,3},
		countDown = 60,--结束倒计时提示
	},
}

genPos = {
	{chapter = 1,	playerPos = {x= 15, y =24}, bossPos = {x= 24, y =27}, monPos = {{x = 20, y=30	}, {x = 25, y=23	}, {x = 25, y=31	}, }, },
	{chapter = 2,	playerPos = {x= 16, y =41}, bossPos = {x= 25, y =32}, monPos = {{x = 25, y=41	}, {x = 29, y=40	}, {x = 31, y=33	}, }, },
	{chapter = 3,	playerPos = {x= 12, y =38}, bossPos = {x= 24, y =31}, monPos = {{x = 17, y=33	}, {x = 21, y=36	}, {x = 25, y=39	}, }, },
	{chapter = 4,	playerPos = {x= 18, y =39}, bossPos = {x= 26, y =29}, monPos = {{x = 19, y=29	}, {x = 22, y=32	}, {x = 26, y=36	}, }, },
	{chapter = 5,	playerPos = {x= 21, y =26}, bossPos = {x= 28, y =39}, monPos = {{x = 17, y=37	}, {x = 22, y=36	}, {x = 25, y=32	}, }, },
	{chapter = 6,	playerPos = {x= 30, y =26}, bossPos = {x= 19, y =35}, monPos = {{x = 18, y=29	}, {x = 24, y=35	}, {x = 24, y=31	}, }, },
	{chapter = 7,	playerPos = {x= 27, y =44}, bossPos = {x= 20, y =24}, monPos = {{x = 17, y=33	}, {x = 21, y=35	}, {x = 24, y=29	}, }, },
	{chapter = 8,	playerPos = {x= 24, y =15}, bossPos = {x= 15, y =27}, monPos = {{x = 20, y=20	}, {x = 11, y=21	}, {x = 23, y=31	}, }, },
	{chapter = 9,	playerPos = {x= 12, y =28}, bossPos = {x= 46, y =32}, monPos = {{x = 41, y=39	}, {x = 35, y=37	}, {x = 28, y=31	}, }, },
	{chapter = 10,	playerPos = {x= 10, y =53}, bossPos = {x= 39, y =43}, monPos = {{x = 23, y=46	}, {x = 26, y=35	}, {x = 32, y=50	}, }, },
	{chapter = 11,	playerPos = {x=  6, y =23}, bossPos = {x= 20, y =25}, monPos = {{x = 13, y=24	}, {x = 16, y=19	}, {x = 14, y=30	}, }, },
	{chapter = 12,	playerPos = {x= 16, y =55}, bossPos = {x= 38, y =24}, monPos = {{x = 26, y=40	}, {x = 25, y=26	}, {x = 39, y=49	}, }, },
	{chapter = 13,	playerPos = {x= 35, y =28}, bossPos = {x= 19, y =32}, monPos = {{x = 25, y=28	}, {x = 12, y=31	}, {x = 28, y=39	}, }, },
	{chapter = 14,	playerPos = {x= 31, y =15}, bossPos = {x= 41, y =30}, monPos = {{x = 36, y=25	}, {x = 50, y=20	}, {x = 30, y=38	}, }, },
	{chapter = 15,	playerPos = {x=  6, y =18}, bossPos = {x= 13, y =23}, monPos = {{x = 14, y=19	}, {x = 18, y=28	}, {x = 10, y=25	}, }, },
	{chapter = 16,	playerPos = {x=  4, y =25}, bossPos = {x= 14, y =11}, monPos = {{x = 11, y=29	}, {x = 21, y=17	}, {x =  9, y=12	}, }, },
}

