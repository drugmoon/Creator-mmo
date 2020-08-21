

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
export default class GameManager extends cc.Component {

    private static _instance: GameManager;
    public static get instance(): GameManager {
        /*if(this._instance == null)
        {
            this._instance = new GameManager();
            this._instance.init();
        }*/
        return GameManager._instance;
    }

    //public controllMode:ControlMode = ControlMode.touch;

    @property(cc.Prefab)
    public playerPrefabArr:cc.Prefab[] = [];


    @property(cc.Prefab)
    public roadSignPrefab:cc.Prefab = null;

    // LIFE-CYCLE CALLBACKS:

    onLoad () 
    {
        if(!GameManager._instance)
        {
            GameManager._instance = this;
            cc.game.addPersistRootNode(this.node);
            this.init();
        }else
        {
            this.node.destroy();
        }
    }

    

    public init()
    {

    }


    /**
     * 获得玩家
     */
    public getPlayer(roleId:number):Player
    {
        var node:cc.Node = cc.instantiate(this.playerPrefabArr[roleId - 1]);
        var player = node.getComponent(Player);
        player.node.position = cc.Vec2.ZERO;
        player.roleId = roleId;
        player.node.active = true;
        player.init();
        console.log("======creat play");
        return player;
    }

    public getRoadSign():RoadSign
    {
        var rd:RoadSign = cc.instantiate(this.roadSignPrefab).getComponent(RoadSign);
        rd.node.active = true;
        rd.node.position = cc.Vec2.ZERO;
        
        return rd;
    }

    /*start () {

    }*/

    // update (dt) {}
}
