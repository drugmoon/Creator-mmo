--
local global = require "global"
local skynet = require "skynet"
local interactive = require "base.interactive"
local basehook = require "base.basehook"
local res = require "base.res"
local record = require "public.record"
local mongoop = require "base.mongoop"
local router = require "base.router"

require "skynet.manager"

local logiccmd = import(service_path("logiccmd.init"))
local routercmd = import(service_path("routercmd.init"))
local demisdk = import(lualib_path("public.demisdk"))
local serverinfo = import(lualib_path("public.serverinfo"))

local no = ...

skynet.start(function()
    interactive.Dispatch(logiccmd)
    router.DispatchC(routercmd)
    local m = serverinfo.get_local_dbs()
    local oClient = mongoop.NewMongoClient({host = m.game.host,port = m.game.port,username = m.game.username,password = m.game.password})
    global.oGameDb = mongoop.NewMongoObj()
    global.oGameDb:Init(oClient, "game")
    global.oDemiSdk = demisdk.NewDemiSdk(true, no)
    skynet.register ".guozi"  -- 划重点 创建skynet模块 guozi 这里只创建了1个线程
    interactive.Send(".dictator", "common", "Register", {type = ".guozi",addr = MY_ADDR,})
    record.info("guozi service booted")
end)
