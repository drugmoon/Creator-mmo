

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
import  Formation ,{ FormationProperty } from "../fight/FormationPosDef"
import GameManager from "../core/GameManager";
import RoadSign from "../gamescene/RoadSign";
import {RoleAttributeVo} from "../vo/RoleAttributeVo";
const {ccclass, property} = cc._decorator;

@ccclass
export default class DataManager extends cc.Component {

    private static _instance: DataManager;
    public static get instance(): DataManager {
        if(this._instance == null)
        {
            this._instance = new DataManager();
            this._instance.init();
        }
        return DataManager._instance;
    }

    // public players_self:Player[] = [];//自己人
    // public players_foe:Player[] = []; //敌人
    public playersSelf:RoleAttributeVo[] = []; //自己人
    public playersFoe:RoleAttributeVo[] = []; //敌人


    // /** role数据 */
    // private playersSelfMap: Map<number, RoleAttributeVo> = new Map();
    // /** role数据 */
    // public get PlayersSelfMap(): Map<number, RoleAttributeVo>
    // {
    //     return this.playersSelfMap;
    // }
    // /** role数据 */
    // private playersFoefMap: Map<number, RoleAttributeVo> = new Map();
    // /** role数据 */
    // public get PlayersFoefMap(): Map<number, RoleAttributeVo>
    // {
    //     return this.playersFoefMap;
    // }

    onLoad () 
    {

    }


    public init()
    {

    }


    public  setDataJoinFight()
    {
        //敌方整容
        //阵型
        //人数 排序
        //人物角色id 装备信息物品  宠物信息
        //hp mp
        // let forma:FormationProperty = Formation.instance.FormationPos[0];

        this.playersSelf[0]._PlayerID = 11;
        
        this.playersFoe[0]._PlayerID = 11;
        // for(var i:number = 0 ; i < 1 ; i++)
        // {
        //     let pos = forma.self[i] ;

        //     this.players_self[i] = GameManager.instance.getPlayer(10);
        //     this.players_self[i].direction = 0;
        //     this.players_self[i].node.parent = this.entityLayer.node;
        //     this.players_self[i].controlMode = GameManager.instance.controllMode;

        //     this.players_self[i].node.position =  cc.v2( pos[0] ,   pos[1] )//spawnPoint.node.position;

        //     if (i == 0)
        //     {
        //         this.player = this.players_self[i]
        //     }
    
        // }

        //己方整容
        //阵型
        //人数 排序
        //人物角色id 装备信息物品  宠物信息
        //hp mp
    }
    public  setDataFightAction(){

    }

    /*start () {

    }*/

    // update (dt) {}
}
