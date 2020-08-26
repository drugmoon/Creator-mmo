import BattlePanel from "../ui/BattlePanel"
import Player from "../gamescene/player/Player"
import Command from "./Command"

import { EventMgr }  from "../common/EventManager";
import { EventType } from "../common/EventType"; 


const {ccclass, property} = cc._decorator;

/**
 * 场景管理器
 */
@ccclass
export default class CommandInterpreter {

    public constructor(battlePanel:BattlePanel){

        this.battlePanel = battlePanel;
    }

    public battlePanel:BattlePanel = null;//

    init()
    {

    }
    start()
    {

    }

    public exce(command:Command){
        this.battlePanel.hidePanel()
    }
	public attack(command:Command) {
        let source:Player = command.getSource();
		let target:Player = command.getTarget();
        let location = source.getLocation();

        //cc.macro.MIN_ZINDEX 和 cc.macro.MAX_ZINDEX 
        //0 设置层级 攻击方显示在上层
        source.node.zIndex = 2;
        target.node.zIndex = 1;

        // //战斗中
        source.attacking = true;
        target.attacking = true;

        //1 描述
        this.battlePanel.setBattleMessage("#Y" + "进行了攻击#18");

        //2 移动到目录位置 急速移动
        let dx =   80;
		let dy =  - 80;

        this.battlePanel.rush(source, target.getLocation().x  + dx, target.getLocation().y  + dy,"",()=>{
            
        //     cc.log("rush finish");
        });
        EventMgr.setEventListener(EventType.BattleAction_Finish_ATTACK,()=>{
            console.log("BattleAction_Finish_ATTACK");
            //动作完成后切换状态
            source.getClip().playTimes = 0;
            source.stop();
            //this._movieClip.reset();
            source.getClip().play();

        },this);

        EventMgr.setEventListener(EventType.BattleAction_Finish_HIT, ()=>{
            console.log("BattleAction_Finish_HIT");
             //动作完成后切换状态
            target.getClip().playTimes = 0;
            target.stop();
             //this._movieClip.reset();
            target.getClip().play();
        },this);



        source.scheduleOnce(()=>{

            //战斗中
            source.attacking = false;
            target.attacking = false;

            //3 攻击动作
            source.attack();


            //target.moveSpeed = 1;
            //4 被攻击
            target.hit();


        },0.3);


    }
}