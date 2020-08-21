import Player from "../gamescene/player/Player";
import GameManager from "../core/GameManager";
import DataManager from "../core/DataManager";
import EntityLayer from "../map/layer/EntityLayer";
import RoadNode from "../map/road/RoadNode"
import CommandManager from "../common/CommandManager"
import Command from "../battle/Command"

import {RoleAttributeVo} from "../vo/RoleAttributeVo";
import {FightInfoVo, AttackType} from "../vo/FightInfoVo";
const {ccclass, property} = cc._decorator;

@ccclass
export default class BattlePanel extends cc.Component {

    // private static _instance: BattlePanel;
    // public static get instance(): BattlePanel {
    //     /*if(this._instance == null)
    //     {
    //         this._instance = new GameManager();
    //         this._instance.init();
    //     }*/
    //     return this._instance;
    // }

    @property(EntityLayer)
    public entityLayer: EntityLayer = null;

    @property(cc.Label)
    public battleMessage: cc.Label = null;

	/** 已方 */
    public ownsideTeam:Player[] = [];
    /** 敌方 */
    public hostileTeam:Player[] = []; 

    private cmdIndex:number = 0;
 
    public commandManager: CommandManager = null;

    onLoad () 
    {

    }

    start () {

    }

    public init()
    {
        this.commandManager = new CommandManager(this);
        this.setOwnsideTeam();
        this.setHostileTeam();

        this.attackCmd();
    }
    /**
	 * 已方团队
	 * 
	 * @param team
	 */
    setOwnsideTeam()
    {
        //this.ownsideTeam = team;
		this.initOwnsideTeam();
    }

    /**
	 * 敌方团队
	 * 
	 * @param team
	 */
    setHostileTeam()
    {
        //this.hostileTeam = team;
		this.initHostileTeam();
    }

    initComponetn()
    {

    }
    initOwnsideTeam()
    {
        let ownsideTeamVo:RoleAttributeVo[] = DataManager.instance.ownsideTeamVo;

		for (let i = 0; i < ownsideTeamVo.length; i++) {
            this.ownsideTeam[i] = GameManager.instance.getPlayer(ownsideTeamVo[i]._PlayerID);
            this.ownsideTeam[i].controlMode = GameManager.instance.controllMode;
            this.ownsideTeam[i].node.position = cc.v2(530 - 55 * i, 560 - 400 - 45 * i);////cc.v2(100,100);//
            this.ownsideTeam[i].direction = 3;

            console.log("ownsideTeam x: "+ this.ownsideTeam[i].node.position.x + " y: "  + this.ownsideTeam[i].node.position.y)
            this.node.addChild(this.ownsideTeam[i].node);
		}
    }
	/**
	 * initHostileTeam(初始化敌方团队)
	 */
    initHostileTeam()
    {
        let dx = 60, dy = 40;
        // int x0 = 340, y0 = 400;
        let x1 = 300, y1 = 150;

		switch (this.hostileTeam.length) {
            case 1:
                x1 -= 2 * dx;
                y1 += 2 * dy;
                break;
            case 2:
                x1 -= 1.5 * dx;
                y1 += 1.5 * dy;
                break;
            case 3:
                x1 -= 1 * dx;
                y1 += 1 * dy;
                break;
            case 4:
                break;
            default:
                break;
            }
            let hostileTeamVo:RoleAttributeVo[] = DataManager.instance.hostileTeamVo;

            for (let i = 0; i < hostileTeamVo.length; i++) {
                
                this.hostileTeam[i] = GameManager.instance.getPlayer(hostileTeamVo[i]._PlayerID);
                this.hostileTeam[i].controlMode = GameManager.instance.controllMode;
                this.hostileTeam[i].node.position = cc.v2(x1 - dx * i, 560 -  (y1 + dy * i));//cc.v2(200,200);
                this.hostileTeam[i].direction = 7;
				//560 - 
                console.log("hostileTeamVo x: "+ this.hostileTeam[i].node.position.x + " y: "  + this.hostileTeam[i].node.position.y)
                this.node.addChild(this.hostileTeam[i].node);
            }
		// ostileTeamVo x: 300 y: 150

		// hostileTeamVo x: 240 y: 190

		// hostileTeamVo x: 180 y: 230
    }

    /**
	 * 当前指定战斗指令的人物序号
	 */

	public addCmd(cmd:Command) {
        this.commandManager.addCmd(cmd);
        this.commandManager.turnBattle();
		// lastCmd = cmd;
		// if (cmdIndex >= ownsideTeam.size() - 1) {
		// 	turnBattle();
		// } else {
		// 	getUIHelp().showPanel(BATTLE_ROLE_CMD);
		// 	cmdIndex++;
		// 	this.setPlayer(ownsideTeam.get(cmdIndex));
		// 	waitingCmd = true;
		// }
	}

	/**
	 * 进入回合战斗
	 */
	private turnBattle() {
        this.commandManager.turnBattle();
		// waitingCmd = false;
		// getUIHelp().hidePanel(BATTLE_ROLE_CMD);
		// new Thread("BattleThread") {
		// 	public void run() {
		// 		commandManager.turnBattle();
		// 	};
		// }.start();
	}

	/**
	 * 新的回合开始
	 */
	public roundStartNew() {
		// Players cmdPlayer = ownsideTeam.get(cmdIndex);
		// waitingCmd = true;
		// getUIHelp().showPanel(BATTLE_ROLE_CMD);
		// cmdIndex = 0;
		// this.setPlayer(ownsideTeam.get(cmdIndex));

    }
    
	/**
	 * 发送攻击命令
	 */
	public attackCmd() {
        let target;
		if (target == null) {
			target = this.randomEnemy();
		}
		let cmdPlayer:Player = this.ownsideTeam[this.cmdIndex];
		let cmd:Command = new Command(Player.STATE_ATTACK, cmdPlayer, target);
		this.addCmd(cmd);
	}

	/**
	 * 施法指令
	 */
	public magicCmd() {
		// if (target == null) {
		// 	target = randomEnemy();
		// }
		// Players cmdPlayer = ownsideTeam.get(cmdIndex);
		// Command cmd = new Command(Players.STATE_MAGIC, cmdPlayer, target);
		// cmd.add(MagicConfig.class.getSimpleName(), magicConfig);
		// addCmd(cmd);
		// playOnceMagic();
	}
    public setBattleMessage(text:string)
    {
        this.battleMessage.string = text;
    }
    /**
	 * @param player
	 * @param x
	 * @param y
	 * @param state
	 *            Players.STATE_RUSHB||Players.STATE_RUSHA
	 */
	public rush(player:Player,  x,  y,  state) {

		var node:RoadNode = new RoadNode();
		
		// node.cx = wPoint.x;
		// node.cy = wPoint.y;
		
		node.px = x;//player.node.position.x;
		node.py = y;//player.node.position.y;
		
		// node.dx = wPoint.x;
		// node.dy = wPoint.y;
        player.walkByRoad([node]);
		// this.targetX = x;
		// this.targetY = y;
		// this.originX = player.getX();
		// this.originY = player.getY();
		// this.movingPlayer = player;
		// player.setState(state);
		// long lastTime = System.currentTimeMillis();
		// while (!isReach()) {
		// 	try {
		// 		Thread.sleep(40);
		// 	} catch (InterruptedException e) {
		// 	}
		// 	long nowTime = System.currentTimeMillis();
		// 	updateMovement(nowTime - lastTime);
		// 	lastTime = nowTime;
		// }
    }
    /**
	 * 随机挑选敌人
	 * 
	 * @return
	 */
	private randomEnemy():Player {
		let target:Player = this.hostileTeam[0];//Math.random()
		return target;
	}
    // battlePanel.setUIhelp(getUIHelp());
    // battlePanel.addBattleListener(battleRoundHandler);
    // battlePanel.setConfigManager(config);
    // battlePanel.setHostileTead((List) args[1]); // 设置敌方队伍
    // battlePanel.setOwnsideTeam((List) args[0]); // 设置己方队伍
    // battlePanel.playMusic();
    // gameFrame.showPanel(battlePanel);
    // new BattlePaneListener(battlePanel);
    // getUIHelp().showPanel(GamePager.class.getSimpleName());
    // battlePanel.initComponetn();
    // update (dt) {}

    public hidePanel() {
		// getUIHelp().hidePanel(BATTLE_ROLE_WARMAGIC);
		// getUIHelp().hidePanel(BATTLE_ROLE_PROP);
		// getUIHelp().hidePanel(BATTLE_ROLE_CMD);
	}
}
