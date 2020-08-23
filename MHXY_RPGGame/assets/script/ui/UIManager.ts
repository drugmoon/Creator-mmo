// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import Joystick from "../gamescene/player/Joystick";
import BattlePanel from "../ui/BattlePanel"
const {ccclass, property} = cc._decorator;

@ccclass
export default class UIManager extends cc.Component {

    private static _instance: UIManager;
    public static get instance(): UIManager {
        /*if(this._instance == null)
        {
            this._instance = new GameManager();
            this._instance.init();
        }*/
        return this._instance;
    }

    @property(Joystick)
    public joystick:Joystick = null;

    @property(cc.Prefab)
    public battlePanelPrefab: cc.Prefab = null;

    @property(cc.Node)
    public battlePanelNode:cc.Node = null;
    
    public battlePanel:BattlePanel = null;
    // LIFE-CYCLE CALLBACKS:

    onLoad () 
    {
        if(!UIManager._instance)
        {
            UIManager._instance = this;
            cc.game.addPersistRootNode(this.node);
            this.init();
        }else
        {
            this.node.destroy();
        }
    }

    start () {

    }

    public init()
    {
        this.node.active = false;


    }

    public initBattlePanel()
    {
        let node:cc.Node = cc.instantiate(this.battlePanelPrefab);
        this.battlePanel = node.getComponent(BattlePanel);

        this.battlePanelNode.addChild(this.battlePanel.node);
    }

    public initFightScene() {

        if (this.battlePanel == null)
        {
            this.initBattlePanel();
            this.battlePanel.init();
        }

    }

    public BattleCommand(){

        this.battlePanel.BattleCommand();
    }

    public cleanFightScene()
    {
        this.battlePanel.node.removeAllChildren();
        this.battlePanel = null;
        //this.battlePanel.reomovPlayer();
    }
    // update (dt) {}
}
