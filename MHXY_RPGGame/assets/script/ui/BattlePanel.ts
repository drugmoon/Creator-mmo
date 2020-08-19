import Player from "../gamescene/player/Player";
import GameManager from "../core/GameManager";
import DataManager from "../core/DataManager";
import EntityLayer from "../map/layer/EntityLayer";

import {RoleAttributeVo} from "../vo/RoleAttributeVo";
import {FightInfoVo, AttackType} from "../vo/FightInfoVo";
const {ccclass, property} = cc._decorator;

@ccclass
export default class BattlePanel extends cc.Component {

    private static _instance: BattlePanel;
    public static get instance(): BattlePanel {
        /*if(this._instance == null)
        {
            this._instance = new GameManager();
            this._instance.init();
        }*/
        return this._instance;
    }

	/** 已方 */
    public ownsideTeam:Player[] = [];
    /** 敌方 */
    public hostileTeam:Player[] = []; 

    @property(EntityLayer)
    public entityLayer: EntityLayer = null;

    onLoad () 
    {

    }

    start () {

    }

    public init()
    {
        this.setOwnsideTeam();
        this.setHostileTeam();
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
            this.ownsideTeam[i].direction = 0;
 
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
                this.hostileTeam[i].node.position = cc.v2(x1 - dx * i, 560 - y1 + dy * i);//cc.v2(200,200);
                this.hostileTeam[i].direction = 0;
 
                console.log("hostileTeamVo x: "+ this.hostileTeam[i].node.position.x + " y: "  + this.hostileTeam[i].node.position.y)
                this.node.addChild(this.hostileTeam[i].node);
            }
            
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
}
