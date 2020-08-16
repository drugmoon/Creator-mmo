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

    @property({type:cc.Enum(PlayerControlType),tooltip:"玩家控制类型:\nnone  无控制 \nuser 用户操作 \nai ai操作 \nnet 网络玩家操作"})
    public controlType:PlayerControlType = PlayerControlType.none;

    @property({type:cc.Enum(ControlMode),tooltip:"控制模式:\ntouch  点击行走 \njoystick 摇杆操作 "})
    public controlMode:ControlMode = ControlMode.touch;

    @property()
    public roleId:number = 0;

    public body:Body = null;

    public moveDir:cc.Vec2 = cc.Vec2.ZERO;

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
                this.movieClip.rowIndex = 0;
            break;

            case 1 : 
                this.movieClip.rowIndex = 4;
            break;

            case 2 : 
                this.movieClip.rowIndex = 1;
            break;

            case 3 : 
                this.movieClip.rowIndex = 6;
            break;

            case 4 : 
                this.movieClip.rowIndex = 3;
            break;

            case 5 : 
                this.movieClip.rowIndex = 7;
            break;

            case 6 : 
                this.movieClip.rowIndex = 2;
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
                this._movieClip = this.node.getChildByName("Body").getChildByName("Skin_Idle").getComponent(MovieClip);
            break;

        }

        
        this.direction = this._direction;
        this._movieClip.node.active = true;
        this._movieClip.playIndex = 0;
        this._movieClip.playAction();

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
