import BattlePanel from "../ui/BattlePanel"
import Player from "../gamescene/player/Player"
import Command from "./Command"
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

        //战斗中
        source.attacking = true;
        target.attacking = true;

        //1 描述
        this.battlePanel.setBattleMessage("#Y" + "进行了攻击#18");

        //2 移动到目录位置 急速移动
        let dx =   45;
		let dy =  - 45 ;

        this.battlePanel.rush(source, target.getLocation().x  + dx, target.getLocation().y  + dy,"",()=>{
            
            cc.log("rush finish");
            


        });

        source.scheduleOnce(()=>{

            //3 攻击动作
            source.attack();


            //4 被攻击
            //target.attack();

            

        },0.3);


    }
}