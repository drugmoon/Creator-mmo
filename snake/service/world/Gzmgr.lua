-- import module  果子后台 功能函数部分  已注释

local global = require "global"
local res = require "base.res"
local cjson = require "cjson"
local record = require "public.record"
local router = require "base.router"
local serverdefines = require "public.serverdefines"

local serverinfo = import(lualib_path("public.serverinfo"))
local analy = import(lualib_path("public.dataanaly"))
local gamedb = import(lualib_path("public.gamedb"))
local loaditem = import(service_path("item.loaditem"))
local loadskill = import(service_path("skill/loadskill"))
local loadpartner = import(service_path("partner/loadpartner"))
local loadsummon = import(service_path("summon.loadsummon"))
local gamedefines = import(lualib_path("public.gamedefines"))

---↑↑↑↑以上加入了部分常用模块接口↑↑↑↑---

function NewGzMgr(...)
    return CGzMgr:New(...)
end

function NewGzCb(...)
    return CGzCb:New(...)
end

CGzMgr = {}
CGzMgr.__index = CGzMgr
inherit(CGzMgr, logic_base_cls())

CGzCb = {}
CGzCb.__index = CGzCb
inherit(CGzCb, logic_base_cls())

function CGzCb:New()
    local o = super(CGzCb).New(self)
    return o
end

function CGzMgr:New()
    local o = super(CGzMgr).New(self)
    o.m_oPayCb = NewGzCb()
    return o
end

function CGzMgr:GzGM(iPid, mOrders)
    if not iPid then 
        return "角色ID不存在" 
    end
    if tonumber(mOrders.type) == 1 then  ---发送邮件 Url http://127.0.0.1:10003/Guozi  Post  Id=10001&item=30600&num=1&type=1
		-- ↑↑↑↑ tonumber转换为整数 tostring转换为文字 获取功能类型 从网页ctype参数传递后获取
	    --示例 函数调用  邮件发送的实现
        local mData = global.oMailMgr:GetMailInfo(9013)  
		-- ↑↑↑↑ 引用已定义的邮件模版来设置邮件标题,内容,类型等 可以在daobiao/gamedata/server/内的data.lua搜寻9013查看具体信息
        local Sid = tonumber(mOrders.item)
		-- ↑↑↑↑ 要发送的物品ID 从网页的item参数传递后获取
        if not Sid then 
            return "物品ID不存在" 
        end
        local Cnt = tonumber(mOrders.num)
		-- ↑↑↑↑ 要发送的物品数量 从网页的num参数传递后获取
		local items = {}
        local oTmpItem = global.oItemLoader:ExtCreate(Sid)
		oTmpItem:SetAmount(Cnt)
		table.insert(items, oTmpItem)
		-- ↑↑↑↑ 加载物品信息并转为数组
		print(tostring(oTmpItem))
        global.oMailMgr:SendMail(0, "果子", iPid, mData, 0, items)
		-- ↑↑↑↑ 调用邮件模块 进行邮件发送
        return "发送邮件成功"
	end
    if tonumber(mOrders.type) == 2 then  ---清空背包 Url http://127.0.0.1:10003/Guozi  Post  Id=10001&type=2
	    local User = global.oWorldMgr:GetOnlinePlayerByPid(iPid)
        if not User then 
            return "失败 角色不在线" 
        end
        for iPos,itemobj in pairs(User.m_oItemCtrl.m_Item) do
            if iPos >= 101 and itemobj then
                local iAmount = itemobj:GetAmount()
                itemobj:AddAmount(-iAmount,"Guozi")
            end
        end
		-- ↑↑↑↑ 循环遍历对象包裹内所有物品并依次清除
        return "清空完毕"
	end
	return "功能调用失败 未选择功能类型"
end
