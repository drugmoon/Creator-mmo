

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

export enum FightState
{
    State_None,		//战斗未开始
    State_Ready,	//战斗准备阶段，接受消息，并且解析
    State_Begin,	//战斗开始准备
    State_Loop,		//战斗循环播放
    State_End,		//战斗结束处理
    State_Wait,		//等待一段时候在开始战斗
}

@ccclass
export default class FightManager extends cc.Component {

    private static _instance: FightManager;
    public static get instance(): FightManager {
        if(this._instance == null)
        {
            this._instance = new FightManager();
            this._instance.init();
        }
        return FightManager._instance;
    }

    public controllMode:ControlMode = ControlMode.touch;

    @property(cc.Prefab)
    public playerPrefabArr:cc.Prefab[] = [];


    @property(cc.Prefab)
    public roadSignPrefab:cc.Prefab = null;

    // LIFE-CYCLE CALLBACKS:

    public fightState:FightState = FightState.State_None;	//战斗流程管理

    public	waitFightBegin:boolean = true;

    onLoad () 
    {

    }

    

    public init()
    {

    }

	public Tick(time)
	{	//时间加速
		//time *= speed;
		
		switch(this.fightState)
		{
		case FightState.State_Ready:	//战斗准备阶段，接受消息，并且解析
			break;
		case FightState.State_Begin:	//战斗开始准备
			this.Process_Begin(time);
			break;
		case FightState.State_Wait:
			this.Process_Wait(time);
			break;
		case FightState.State_Loop:		//战斗循环播放
			this.Process_Loop(time);
			break;
		case FightState.State_End:		//战斗结束处理
			this.Process_End(time);
			break;
		case FightState.State_None:		//战斗未开始
		default:
			break;
		}
	}

    public Process_Begin(time)
    {
		//如果需要等待，无限等待
        if(!this.waitFightBegin) return;
        
        //隐藏界面


        //初始化伤害面板
        //根据战斗人数最多的一方*2+CONST_VALUE.MAX_DAMAGE_SKILL的数量初始化面板
        
        //解析命令

        //消息执行完毕，进入战斗循环
        this.fightState = FightState.State_Wait;
        
    }
    public Process_Wait(time)
    {
        {
			//隐藏界面
 
		}
		{
 
 
        }
        // 发送信息进入战斗
        // 更新ui

        this.fightState = FightState.State_Loop;
		// ModifyState(FightState.State_Loop);
    }
    public Process_Loop(time){}
    public Process_End(time){}

    // public set ModifyState(state:FightState)
	// {
	// 	this.fightState = state;
	// }
    /*start () {

    }*/

    // update (dt) {}
}
