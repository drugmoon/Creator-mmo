

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
import {FightInfoVo} from "../vo/FightInfoVo";
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
    public ownsideTeamVo:RoleAttributeVo[] = []; //自己人
    public hostileTeamVo:RoleAttributeVo[] = []; //敌人

    public _fightInfoVo:FightInfoVo[] = []; //战斗信息

    // /** role数据 */
    // private ownsideTeamVoMap: Map<number, RoleAttributeVo> = new Map();
    // /** role数据 */
    // public get ownsideTeamVoMap(): Map<number, RoleAttributeVo>
    // {
    //     return this.ownsideTeamVoMap;
    // }
    // /** role数据 */
    // private hostileTeamVofMap: Map<number, RoleAttributeVo> = new Map();
    // /** role数据 */
    // public get hostileTeamVofMap(): Map<number, RoleAttributeVo>
    // {
    //     return this.hostileTeamVofMap;
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
        console.log("setDataJoinFight ");

        let RoleAttr_S = new RoleAttributeVo;
        RoleAttr_S._ID = 1001;
        RoleAttr_S._PlayerID = 10;

        let RoleAttr_S_1 = new RoleAttributeVo;
        RoleAttr_S_1._ID = 1002;
        RoleAttr_S_1._PlayerID = 10;

        let RoleAttr_S_2 = new RoleAttributeVo;
        RoleAttr_S_2._ID = 1003;
        RoleAttr_S_2._PlayerID = 10;


        let RoleAttr_F = new RoleAttributeVo;
        RoleAttr_F._ID = 2001;
        RoleAttr_F._PlayerID = 10;

        let RoleAttr_F_1 = new RoleAttributeVo;
        RoleAttr_F_1._ID = 2002;
        RoleAttr_F_1._PlayerID = 10;

        let RoleAttr_F_2 = new RoleAttributeVo;
        RoleAttr_F_2._ID = 2003;
        RoleAttr_F_2._PlayerID = 10;


        this.ownsideTeamVo[RoleAttr_S._Idx] = RoleAttr_S;
        this.ownsideTeamVo[RoleAttr_S_1._Idx + 1] = RoleAttr_S_1;
        this.ownsideTeamVo[RoleAttr_S_2._Idx + 2] = RoleAttr_S_2;


        this.hostileTeamVo[RoleAttr_F._Idx] = RoleAttr_F;
        this.hostileTeamVo[RoleAttr_F_1._Idx + 1] = RoleAttr_F_1;
        this.hostileTeamVo[RoleAttr_F_2._Idx + 2] = RoleAttr_F_2;
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

        //  A > B
        //A攻击B
        //a 攻击动作
        //b 受伤动作
        console.log("setDataFightAction ");

        let fightInfoVo = new FightInfoVo;

        //A>B  B hp - 999
        fightInfoVo._ID_Att = 1001;
        fightInfoVo._ID_Def = 1002;

        //普通攻击
        fightInfoVo._Type = 0;

        //Hp
        fightInfoVo._Hp_att = 0;
        fightInfoVo._Hp_def = -999;

        //Mp
        fightInfoVo._Mp_att = 0;
        fightInfoVo._Mp_def = 0;

        this._fightInfoVo.push(fightInfoVo);
    }

    /*start () {

    }*/

    // update (dt) {}
}
