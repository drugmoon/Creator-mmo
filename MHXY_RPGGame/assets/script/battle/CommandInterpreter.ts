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

        //描述
        this.battlePanel.setBattleMessage("#Y" + "进行了攻击#18");

        //移动到目录位置
        this.battlePanel.rush(source, target.getLocation().x,target.getLocation().y,"");


    }
}