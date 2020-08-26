--import module 果子后台模块 已注释
local global = require "global"
local skynet = require "skynet"
local router = require "base.router"

function GzGM(mRecord, mData)
    local mSg = global.oGzMgr:GzGM(mData.iPid, mData.mOrders) --调用模块 GzMgr
    router.Response(mRecord.srcsk, mRecord.src, mRecord.session, {ret = mSg,}) --回调函数返回信息
end