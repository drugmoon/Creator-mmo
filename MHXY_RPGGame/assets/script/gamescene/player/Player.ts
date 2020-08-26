// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html




import Body from "./Body";
import TransferDoor from "../transfer/TransferDoor";
import RoadNode from "../../map/road/RoadNode";
import SceneMap from "../../SceneMap";
import Character, { CharacterState } from "../../map/character/Character";
import MovieClip from "../../map/character/MovieClip";
import Point from "../../map/road/Point"
import { EventMgr }  from "../../common/EventManager";
import { EventType } from "../../common/EventType"; 

const {ccclass, property} = cc._decorator;


export enum PlayerControlType
{
    /**
     * 无控制
     */
    none = 0,

    /**
     * 用户控制
     */
    user = 1,

    /**
     * ai控制
     */
    ai = 2,
    /**
     * 网络玩家控制
     */
    net = 3,
}

/**
 * 控制模式
 */
export enum ControlMode
{
    /**
     * 点击行走
     */
    touch = 0,

    /**
     * 摇杆操作
     */
    joystick = 1
}

@ccclass
export default class Player extends Character {
	/** 攻击 */
	public static  STATE_ATTACK = "attack";
	/** 施法 */
	public static  STATE_MAGIC = "magic";
	/** 站立 */
	public static  STATE_STAND = "stand";
	/** 防御 */
	public static  STATE_DEFEND = "defend";
	/** 被击中 */
	public static  STATE_HIT = "hit";
	/** 倒下 */
	public static  STATE_DIE = "die";
	/** 移动 */
	public static  STATE_WALK = "walk";
	/** 哭泣 **/
	public static  STATE_WEEP = "weep";
	/** 舞蹈 */
	public static  STATE_DANCE = "dance";
	/** 发怒 */
	public static  STATE_ANGRY = "angry";
	/** 跑去 */
	public static  STATE_RUSHA = "rusha";
	/** 跑回 */
	public static  STATE_RUSHB = "rushb";
	/** 待战 */
	public static  STATE_WRITBUTTLE = "writbuttle";
	/** 人物坐骑站立 */
	public static  STATE_MOUNT_STAND = "mountstand";
	/** 人物坐骑移动 */
    public static  STATE_MOUNT_WALK = "mountwalk";
    
    @property({type:cc.Enum(PlayerControlType),tooltip:"玩家控制类型:\nnone  无控制 \nuser 用户操作 \nai ai操作 \nnet 网络玩家操作"})
    public controlType:PlayerControlType = PlayerControlType.none;

    @property({type:cc.Enum(ControlMode),tooltip:"控制模式:\ntouch  点击行走 \njoystick 摇杆操作 "})
    public controlMode:ControlMode = ControlMode.touch;

    @property()
    public roleId:number = 0;

    public body:Body = null;

    public moveDir:cc.Vec2 = cc.Vec2.ZERO;
    
    public pos:Point = new Point(0,0);
    public getLocation()
    {
        return this.node.position;//this.pos;
    }

    public setLocation(x,y)
    {
        this.pos = new Point(x,y);
    }

    public get movieClip():MovieClip
    {
        if(!this._movieClip)
        {
            this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Idle").getComponent(MovieClip);
        }
        return this._movieClip;
    }

    public set direction(value:number)
    {
        this._direction = value;

        switch(this._direction)
        {
            case 0 : 
                this.movieClip.rowIndex = 0;//正面
            break;

            case 1 : 
                this.movieClip.rowIndex = 4;//右一
            break;

            case 2 : 
                this.movieClip.rowIndex = 1;//正左
            break;

            case 3 : 
                this.movieClip.rowIndex = 6;
            break;

            case 4 : 
                this.movieClip.rowIndex = 3;//正左左左
            break;

            case 5 : 
                this.movieClip.rowIndex = 7;
            break;

            case 6 : 
                this.movieClip.rowIndex = 2;//正左左
            break;

            case 7 : 
                this.movieClip.rowIndex = 5;
            break;
        }

    }

    public set state(value:CharacterState)
    {
        if(this._state == value)
        {
            return;
        }

        this._state = value;

        if(this._movieClip)
        {
            this._movieClip.node.active = false;
        }

        switch(this._state)
        {
            case CharacterState.idle: 
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Idle").getComponent(MovieClip);
            break;

            case CharacterState.walk: 
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Walk").getComponent(MovieClip);
            break;

            case CharacterState.attack: 
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Attack").getComponent(MovieClip);
            break;
            case CharacterState.magic: 
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Magic").getComponent(MovieClip);
            break;
            case CharacterState.hit: 
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Hit").getComponent(MovieClip);
            break;
        }
        console.log("set _state ", this._state);
        console.log("set _direction ", this._direction);
        this.direction = this._direction;
        this._movieClip.node.active = true;
        this._movieClip.playIndex = 0;

        //如果是攻击只播发一次
        if(this._state == CharacterState.attack)
        {
            //获取攻击方向
            this.setAttackDir();

            console.log("setAttackDir state ", this.movieClip.rowIndex);

            this._movieClip.characterState = this._state;
            this._movieClip.playTimes = 0;
            this._movieClip.play();
            this._movieClip.reset();
            this._movieClip.playActionOnce();

        }
        else if(this._state == CharacterState.hit)
        {
            //获取攻击方向
            this.setHitDir();

            console.log("setHitDir state ", this.movieClip.rowIndex);

            this._movieClip.characterState = this._state;
            this._movieClip.playTimes = 0;
            this._movieClip.play();
            this._movieClip.reset();
            this._movieClip.playActionOnce();
        }
        else
        {

            this._movieClip.playAction();
        }
    }

    public getClip()
    {
        return this._movieClip;
    }
    private setAttackDir()
    {
        switch(this._direction)
        {
            case 3 : 
                this.movieClip.rowIndex = 2;//正面
            break;
            case 7 : 
                this.movieClip.rowIndex = 0;//正左左
            break;
        }
    }

    private setHitDir()
    {
        switch(this._direction)
        {
            case 3 : 
                this.movieClip.rowIndex = 1;//正面
            break;
            case 7 : 
                this.movieClip.rowIndex = 0;//正左左
            break;
        }
    }

    private _lastStandNode:RoadNode = null;

    private targetPos:cc.Vec2 = cc.Vec2.ZERO;

    // LIFE-CYCLE CALLBACKS:

    onLoad () { 
        this.body = this.getComponentInChildren(Body);
        
    }

    start () {
    
        super.start();


        
    }

    update(dt)
    {
        super.update(dt);

        if(this.controlMode == ControlMode.joystick)
        {
            if(this.moveDir.x != 0 || this.moveDir.y != 0)
            {
                /*if(this._lastStandNode != this.roadNode)
                {
                    var targetPos:cc.Vec2 = cc.v2(this.roadNode.px,this.roadNode.py).add(this.moveDir.mul(120));
                    SceneMap.instance.movePlayer(targetPos.x,targetPos.y,5);
                }

                this._lastStandNode = this.roadNode;*/

                //this.setFaceDir(this.moveDir);

                var speed:number = this.moveSpeed * dt;
                console.log("speed " + speed);
                var pos:cc.Vec2 = this.node.position.add(this.moveDir.mul(speed));

                var nextNode:RoadNode = SceneMap.instance.getMapNodeByPixel(pos.x,pos.y);

                if(!nextNode)
                {
                    this.targetPos = this.node.position;
                }else
                {
                    if(nextNode.value != 1)
                    {
                        this.targetPos = pos;
                    }else
                    {
                        var nodeArr:RoadNode[] = this.getRoundRoadNodes();
    
                        var bestRoadNode:RoadNode = null;
    
                        for(var i:number = 0 ; i < nodeArr.length ; i++)
                        {
                            if(!nodeArr[i] || nodeArr[i].value == 1 || nodeArr[i] == nextNode)
                            {
                                continue;
                            }
    
                            nodeArr[i].h = (Math.abs(nextNode.cx - nodeArr[i].cx) + Math.abs(nextNode.cy - nodeArr[i].cy)) * 10;
    
                            if(!bestRoadNode)
                            {
                                bestRoadNode = nodeArr[i];
                            }else
                            {
                                if(nodeArr[i].h < bestRoadNode.h)
                                {
                                    bestRoadNode = nodeArr[i];
                                }else if(nodeArr[i].h == bestRoadNode.h)
                                {
                                        var dir1:cc.Vec2 = cc.v2(nodeArr[i].px,nodeArr[i].py).sub(this.node.position).normalize();
                                        var dir2:cc.Vec2 = cc.v2(bestRoadNode.px,bestRoadNode.py).sub(this.node.position).normalize();
    
                                        if((dir1.add(this.moveDir)).mag() > (dir2.add(this.moveDir)).mag())
                                        {
                                        bestRoadNode = nodeArr[i];
                                        }
    
                                }
                            }
                        }
    
                        if(bestRoadNode)
                        {
                            this.targetPos = cc.v2(bestRoadNode.px,bestRoadNode.py);
                        }
                    }
                }

                

                if(this.state != CharacterState.walk)
                {
                    this.state = CharacterState.walk;
                }

                var dir:cc.Vec2 = this.targetPos.sub(this.node.position);
                var dis:number = dir.mag();
                dir = dir.normalize();

                this.setFaceDir(this.moveDir);
                
                if(dis < speed)
                {
                    this.node.position = this.targetPos;
                }else
                {
                    this.node.position = this.node.position.add(dir.mul(speed))
                }

            }else
            {
                this._lastStandNode = null;
                this.stop();
            }
        }
    }

    public onCollisionEnter(other:cc.Collider,self:cc.Collider)
    {
        if(other.tag == 2)
        {
            var transferDoor:TransferDoor = other.getComponent(TransferDoor);
            transferDoor.onTriggerEnter(this);
        }
    }

    

}
