import MovieClip from "./MovieClip";
import RoadNode from "../road/RoadNode";
import SceneMap from "../../SceneMap";
import { EventMgr }  from "../../common/EventManager";
import { EventType } from "../../common/EventType"; 
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

export enum CharacterState
{
    none = 0,
    /**
     * 待机
     */
    idle = 1,

    /**
     * 行走
     */
    walk = 2,

    /**
     * 攻击
     */
    attack = 3,
}

/**
 * 场景角色 
 * @author 落日故人 QQ 583051842
 * 
 */

@ccclass
export default class Character extends cc.Component {

    protected _movieClip:MovieClip = null;

    public get movieClip():MovieClip
    {
        if(!this._movieClip)
        {
            this._movieClip = this.node.getComponentInChildren(MovieClip);
        }
        return this._movieClip;
    }


    protected _direction:number = 0;

    public get direction():number
    {
        return this._direction;
    }

    public set direction(value:number)
    {
        this._direction = value;
    }

    protected _state:CharacterState = CharacterState.none;

    public get state():CharacterState
    {
        return this._state;
    }

    public set state(value:CharacterState)
    {
        this._state = value;
    }

    protected _alpha: number = 1;
    public get alpha(): number {
        return this._alpha;
    }
    public set alpha(value: number) {
        this._alpha = value;
        this.node.opacity = Math.floor(255 * (value/1))
    }

    /**
     * 角色当前所站在的路点
     */
    public get roadNode():RoadNode
    {
        return SceneMap.instance.getMapNodeByPixel(this.node.x,this.node.y);
    }



    /**
     *玩家当前所站在的地图节点 
     */		
    protected _currentNode:RoadNode;

    //public isScrollScene:boolean = false;

    public moving:boolean = false;

  //  public attacking:boolean = false;

    @property()
    public moveSpeed:number = 200;

    protected _moveAngle:number = 0;

    protected _roadNodeArr:RoadNode[] = [];
    protected _nodeIndex:number = 0;


    /**
     *用于检索一个节点周围8个点的向量数组 
    */		
    protected _round:number[][] = [[0,-1],[1,-1],[1,0],[1,1],[0,1],[-1,1],[-1,0],[-1,-1]];
    
    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    init() {
        this.direction = 0;
        
    }
    start () {

        
        this.node.getChildByName("Body").getChildByName("Skin_Idle").active = false;
        this.node.getChildByName("Body").getChildByName("Skin_Walk").active = false;
        if(this.node.getChildByName("Body").getChildByName("Skin_Attack")) this.node.getChildByName("Body").getChildByName("Skin_Attack").active = false;
        this.state = CharacterState.idle;

    }
    public set attacking(value:boolean) {

        if (value)
        {
            this.moveSpeed = 1000;
        }
        else
        {
            this.moveSpeed = 200;
        }
    }
    update (dt) 
    {
        if(this.moving)
        {
            var nextNode:RoadNode = this._roadNodeArr[this._nodeIndex];
            var dx:number = nextNode.px - this.node.x;
            var dy:number = nextNode.py - this.node.y;

            var speed:number = this.moveSpeed * dt;
            console.log("speed " + speed);
            console.log("this.moveSpeed " + this.moveSpeed);
            if(dx * dx + dy * dy > speed * speed)
            {
                if(this._moveAngle == 0)
                {
                    this._moveAngle = Math.atan2(dy,dx);

                    var dire:number = Math.round((-this._moveAngle + Math.PI)/(Math.PI / 4));
                    this.direction = dire > 5 ? dire-6 : dire+2;
                }

                var xspeed:number = Math.cos(this._moveAngle) * speed;
                var yspeed:number = Math.sin(this._moveAngle) * speed;

                this.node.x += xspeed;
                this.node.y += yspeed;

            }else
            {
                this._moveAngle = 0;

                if(this._nodeIndex == this._roadNodeArr.length - 1)
                {
                    this.node.x = nextNode.px;
                    this.node.y = nextNode.py
                    this.stop();
                }else
                {
                    this.walk();
                }
            }
        }

        this.setPlayerStateByNode();

    }

    public setPlayerStateByNode():void
    {
        var node:RoadNode = this.roadNode;
        
        if(node == this._currentNode)
        {
            return;
        }
        
        this._currentNode = node
        
        if(this._currentNode)
        {
            switch(this._currentNode.value)
            {
                case 2://如果是透明节点时
                    if(this.alpha != 0.4)
                    {
                        this.alpha = 0.4;
                    }
                    break;
                case 3://如果是透明节点时
                    //trace("走到该节点传送");
                    //this.alpha < 1 && (this.alpha = 1);
                    this.alpha > 0 && (this.alpha = 0);
                    break;
                default:
                    this.alpha < 1 && (this.alpha = 1);
                    
            }
            
        }

    }

    /**
     * 玩家朝向某个点
     * @param px 
     * @param py 
     */
    public rotateToPos(px:number,py:number)
    {
        var dx:number = px - this.node.x;
        var dy:number = py - this.node.y;

        this._moveAngle = Math.atan2(dy,dx);

        var dire:number = Math.round((-this._moveAngle + Math.PI)/(Math.PI / 4));
        this.direction = dire > 5 ? dire-6 : dire+2;

    }

    /**
     * 
     * @param 设置玩家朝向
     */
    public setFaceDir(dir:cc.Vec2)
    {
        this._moveAngle = Math.atan2(dir.y,dir.x);

        var dire:number = Math.round((-this._moveAngle + Math.PI)/(Math.PI / 4));
        this.direction = dire > 5 ? dire-6 : dire+2;
    }

    /**
     * 根据路节点路径行走
     * @param roadNodeArr 
     */
    public walkByRoad(roadNodeArr:RoadNode[], callback:Function = null)
    {
        this._roadNodeArr = roadNodeArr;
        this._nodeIndex = 0;
        this._moveAngle = 0;

        this.walk();
        this.move();

        // EventMgr.addEventListener(EventType.BattleAction_Finish_MOVE, ()=>{
        //     console.log("BattleAction_Finish_MOVE");
        //     callback();
        // })
        
        
    }

    private walk()
    {
        if(this._nodeIndex < this._roadNodeArr.length - 1)
        {
            this._nodeIndex ++;
        }else
        {

        }
    }
    
    public attack()
    {
        this.state = CharacterState.attack;
    }

    public move()
    {
        this.moving = true;
        this.state = CharacterState.walk;
    }

    public stop()
    {
        this.moving = false;
        this.state = CharacterState.idle;
    }

    /**
     *获得角色周围相邻的所有路点
    * @param node
    * @return 
    * 
    */		
    public getRoundRoadNodes():RoadNode[]
    {
        var nodeArr:RoadNode[] = [];
        var node:RoadNode = this.roadNode;

        for(var i:number = 0 ; i < this._round.length ; i++)
        {
            var cx:number = node.cx + this._round[i][0];
            var cy:number = node.cy + this._round[i][1];

            nodeArr.push(SceneMap.instance.getMapNodeByWorldPoint(cx,cy));
        }

        return nodeArr;
    }


}
