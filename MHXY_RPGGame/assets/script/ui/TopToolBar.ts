// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import SceneMap from "../SceneMap";
import Player, { ControlMode } from "../gamescene/player/Player";
import GameManager from "../core/GameManager";

const {ccclass, property} = cc._decorator;

@ccclass
export default class TopToolBar extends cc.Component {

    @property(cc.Button)
    controllBtn: cc.Button = null;

    @property(cc.Button)
    swBtn: cc.Button = null;

    private playerIndex:number = 0;

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

        if(GameManager.instance.controllMode == ControlMode.touch)
        {
            this.controllBtn.getComponentInChildren(cc.Label).string = "切换地图点击控制";
        }else if(GameManager.instance.controllMode == ControlMode.joystick)
        {
            this.controllBtn.getComponentInChildren(cc.Label).string = "切换摇杆控制";
        }

        this.controllBtn.node.on(cc.Node.EventType.TOUCH_START,(event:cc.Event.EventTouch)=>
        {
            var lastPlayer:Player = SceneMap.instance.player;
            
            if(GameManager.instance.controllMode == ControlMode.touch)
            {
                GameManager.instance.controllMode = ControlMode.joystick;
                this.controllBtn.getComponentInChildren(cc.Label).string = "切换地图点击控制";
            }else if(GameManager.instance.controllMode == ControlMode.joystick)
            {
                GameManager.instance.controllMode = ControlMode.touch;
                this.controllBtn.getComponentInChildren(cc.Label).string = "切换摇杆控制";
            }

            lastPlayer.controlMode = GameManager.instance.controllMode;

            
        });

        this.swBtn.node.on(cc.Node.EventType.TOUCH_START,(event:cc.Event.EventTouch)=>
        {
            var lastPlayer:Player = SceneMap.instance.player;
            this.playerIndex++;
            this.playerIndex %= 11;
            SceneMap.instance.player = GameManager.instance.getPlayer(this.playerIndex + 1);
            SceneMap.instance.player.node.parent = lastPlayer.node.parent;
            SceneMap.instance.player.node.position = lastPlayer.node.position;
            SceneMap.instance.player.state = lastPlayer.state;
            SceneMap.instance.player.controlMode = lastPlayer.controlMode;

            lastPlayer.node.destroy();

        });

    }

    // update (dt) {}
}
