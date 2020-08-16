import Player, {PlayerControlType, ControlMode } from "./Player";
import UIManager from "../../ui/UIManager";
import Joystick from "./Joystick";
import SceneMap from "../../SceneMap";


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



@ccclass
export default class PlayerController extends cc.Component {


    public player:Player = null;

    private touchPos:cc.Vec2 = cc.Vec2.ZERO;

    private joyStick:Joystick = null;

    // LIFE-CYCLE CALLBACKS:

    onLoad () 
    {

    }

    start () {

        this.player = this.getComponent(Player);


        //this.player.controlMode = ControlMode.joystick; //摇杆操作
        //this.player.controlMode = ControlMode.touch; //点击操作

        this.joyStick = UIManager.instance.joystick;
        
        
        if(this.player.controlType != PlayerControlType.user)
        {
            return;
        }
        
        var touchPlane:cc.Node = SceneMap.instance.mapLayer.node;

        touchPlane.on(cc.Node.EventType.TOUCH_START,this.onTouchMap,this);

        touchPlane.on(cc.Node.EventType.TOUCH_START,this.onJoystickTouchStart,this);
    
        touchPlane.on(cc.Node.EventType.TOUCH_MOVE,this.onJoystickTouchMove,this);

        touchPlane.on(cc.Node.EventType.TOUCH_END,this.onJoystickTouchEnd,this);

        touchPlane.on(cc.Node.EventType.TOUCH_CANCEL,this.onJoystickTouchEnd,this);
        
    }

    public onTouchMap(event:cc.Event.EventTouch)
    {
        if(this.player.controlMode == ControlMode.touch)
        {
            SceneMap.instance.onMapMouseDown(event);
        }
    }

    public onJoystickTouchStart(event:cc.Event.EventTouch)
    {
        if(this.player.controlMode == ControlMode.joystick)
        {
            this.touchPos = event.getLocation();
            this.joyStick.node.position = cc.v2(event.getLocation().x - cc.winSize.width * 0.5,event.getLocation().y - cc.winSize.height * 0.5);
            this.joyStick.show();
        }
    }

    public onJoystickTouchMove(event:cc.Event.EventTouch)
    {
        if(this.player.controlMode == ControlMode.joystick)
        {
            var currentPos = event.getLocation();
            this.player.moveDir = currentPos.sub(this.touchPos).normalize();
            this.joyStick.cursorTo(this.player.moveDir);
        }
    }

    public onJoystickTouchEnd(event:cc.Event.EventTouch)
    {
        if(this.player.controlMode == ControlMode.joystick)
        {
            this.player.moveDir = cc.Vec2.ZERO;
            this.player.stop();
            this.joyStick.hidden();
        }
    }


    update (dt) 
    {

    }

    onDestroy()
    {
        var touchPlane:cc.Node = SceneMap.instance.mapLayer.node;

        if(touchPlane)
        {
            touchPlane.off(cc.Node.EventType.TOUCH_START,this.onTouchMap,this);


            //joystick
    
            touchPlane.off(cc.Node.EventType.TOUCH_START,this.onJoystickTouchStart,this);
        
            touchPlane.off(cc.Node.EventType.TOUCH_MOVE,this.onJoystickTouchMove,this);
    
            touchPlane.off(cc.Node.EventType.TOUCH_END,this.onJoystickTouchEnd,this);
    
            touchPlane.off(cc.Node.EventType.TOUCH_CANCEL,this.onJoystickTouchEnd,this);
        }


    }

}
