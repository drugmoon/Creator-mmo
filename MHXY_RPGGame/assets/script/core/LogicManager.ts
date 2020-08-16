

// Learn TypeScript:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import Player, { ControlMode } from "../gamescene/player/Player";
import RoadSign from "../gamescene/RoadSign";

const {ccclass, property} = cc._decorator;

@ccclass
export default class LogicManager extends cc.Component {

    private static _instance: LogicManager;
    public static get instance(): LogicManager {
        if(this._instance == null)
        {
            this._instance = new LogicManager();
            this._instance.init();
        }
        return LogicManager._instance;
    }


    onLoad () 
    {

    }


    public init()
    {

    }

    public Req_JoinFight() {

    }
    public Req_FightAction() {
        
    }

    public Ack_JoinFight()
    {

    }
    public Ack_FightAction()
    {
        
    }
    /*start () {

    }*/

    // update (dt) {}
}
