

// Learn TypeScript:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import { EventMgr }  from "../common/EventManager";
import LogicManager from "./LogicManager"
import DataManager from "./DataManager";
const {ccclass, property} = cc._decorator;

@ccclass
export default class MsgManager extends cc.Component {

    private static _instance: MsgManager;
    public static get instance(): MsgManager {
        if(this._instance == null)
        {
            this._instance = new MsgManager();
            this._instance.init();
        }
        return MsgManager._instance;
    }


    onLoad () 
    {

    }


    public init()
    {
        //监听消息
        this.Listener()
        //处理消息

        
    }

    public Listener(){

        
        //接受到进入战斗消息 基础信息
        // Req_JoinFight
        EventMgr.addEventListener("Ack_JoinFight",this.recvMsg,1)

        //战斗动作
        //Req_FightAction

        //
        EventMgr.addEventListener("Ack_FightAction",this.recvMsg,2)
        

    }

    public sendMsg(){

    }
    public recvMsg(target){

        switch(target)
        {
            case 1:
                DataManager.instance.setDataJoinFight();
            break;

            case 2: 
                DataManager.instance.setDataFightAction();
            break;
        

        }

    }


    /*start () {

    }*/

    // update (dt) {}
}
