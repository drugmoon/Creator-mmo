import Command from "../battle/Command"
import CommandInterpreter from "../battle/CommandInterpreter"
import BattlePanel from "../ui/BattlePanel"

const {ccclass, property} = cc._decorator;

/**
 * 场景管理器
 */
@ccclass
export default class CommandManager {

    public constructor(battlePanel:BattlePanel){

        this.battlePanel = battlePanel;
        this.interpretor = new CommandInterpreter(this.battlePanel);
    }
    public cmdQueue:Command[] = [];//自己人
    public interpretor:CommandInterpreter = null;//
    public battlePanel:BattlePanel = null;//

    init(){


    }   
    start()
    {

    }
	/** 战斗开始 */
	public turnBattle() {

        for(let i = 0; i < this.cmdQueue.length; i ++)
        {
            this.interpretor.attack(this.cmdQueue[i]);
        }
		// battlePanel.fireBattleEvent(new BattleEvent(battlePanel,BattleEvent.BATTLE_WIN));
		// battlePanel.getTimerManager().cleanCountDown();
		// turnBegin();
		// for (Command command : cmdQueue) {
		// 	this.interpretor.exce(command);
		// }
		// turnEnd();
	}

	public turnEnd() {
		// cmdQueue.clear();
		// battlePanel.roundStartNew();
		// battlePanel.getTimerManager().countDown();
	}
    public addCmd(cmd:Command) {
		this.cmdQueue.push(cmd);
	}
}