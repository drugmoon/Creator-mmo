// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

const {ccclass, property} = cc._decorator;

/**
 * 剧情管理器
 */
@ccclass
export default class PlotManager extends cc.Component {

    private static _instance: PlotManager;
    public static get instance(): PlotManager {
        /*if(this._instance == null)
        {
            this._instance = new GameManager();
            this._instance.init();
        }*/
        return this._instance;
    }

    public isFirstGame:boolean = true;

    @property(cc.Node)
    dialogue: cc.Node = null;

    @property(cc.Label)
    dialogueMsg: cc.Label = null;

    // LIFE-CYCLE CALLBACKS:

    onLoad () 
    {
        PlotManager._instance = this;
        this.dialogue.active = false;
    }

    start () {

    }

    // update (dt) {}

    public showMsg(msg:string)
    {
        cc.log("剧情",msg);
        this.dialogue.active = true;
        this.dialogueMsg.string = msg;
        this.scheduleOnce(()=>{
            this.dialogue.active = false;
        },3.5);
    }
}
