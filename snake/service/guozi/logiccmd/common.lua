--import module  果子后台模块 已注释
local global = require "global"
local skynet = require "skynet"
local interactive = require "base.interactive"
local router = require "base.router"

function Guozi(mRecord, mData) -- 创建函数 后台接口
    local iPid = tonumber(mData.Id)  -- 获取提交数据中的 Id 参数 
	router.Request("gs10001", ".world", "Gz", "GzGM", {iPid=iPid,mOrders=mData}, -- 利用 router 交互传参给word模块内自定义接口 Gz 内的 GzGM 函数
	function(mRecords, mDatas)
		interactive.Response(mRecord.source, mRecord.session, {ret= mDatas.ret})  -- 获取函数返回信息回调给网页服务
	end)
end