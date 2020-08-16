// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import SceneMap from "../../SceneMap";
import Point from "../../map/road/Point";
import RoadNode from "../../map/road/RoadNode";
import MapRoadUtils from "../../map/road/MapRoadUtils";
import SceneManager from "../../SceneManager";
import Character, { CharacterState } from "../../map/character/Character";

const {ccclass, property} = cc._decorator;

@ccclass
export default class NPC extends Character {

    public get className()
    {
        return NPC.name;
    }

    @property()
    public npcId:number = 0;

    @property()
    public isPatrol:boolean = true;

    @property()
    public patrolRange:number = 200;

    @property()
    public defaultDir:number = 0;

    private _objId: number = 0;
    public get objId(): number {
        return this._objId;
    }
    public set objId(value: number) {
        this._objId = value;
    }

    public set direction(value:number)
    {
        this._direction = value;

        if(value > 4)
        {
            this.movieClip.rowIndex = 4 - value % 4;
            this.movieClip.node.scaleX = -1;
        }else
        {
            this.movieClip.rowIndex = value;
            this.movieClip.node.scaleX = 1;
        }
    }

    public set state(value:CharacterState)
    {
        this._state = value;

        var halfCol:number = this.movieClip.col / 2;

        switch(this._state)
        {
            case CharacterState.idle: 
                this.movieClip.begin = 0;
                this.movieClip.end = halfCol;
            break;

            case CharacterState.walk: 
                this.movieClip.begin = halfCol;
                this.movieClip.end = this.movieClip.col;
            break;
        }

    }

    private basePos:cc.Vec2 = null;

    private targetPos:cc.Vec2 = new cc.Vec2();

    private timer:number = 3.5;

    onLoad () 
    {
        if(this.node.getChildByName("AnchorPoint"))
        {
            this.node.getChildByName("AnchorPoint").active = false;
        }

        this.movieClip.node.active = false;
        this.basePos = this.node.position;
        this.timer = this.Range(0.5 , 1.5);

        this.targetPos.x = this.basePos.x + this.Range(-this.patrolRange , this.patrolRange);
        this.targetPos.y = this.basePos.y + this.Range(-this.patrolRange , this.patrolRange);
    }

    start () {


        this.direction = this.defaultDir;

        this.node.width = 100;
        this.node.height = 100;

        //隐藏NPC
        // if(this.npcId != 0)
        // {
        //     cc.loader.loadRes("game/npc/" + this.npcId, cc.Texture2D,(error:Error,tex:cc.Texture2D)=>
        //     {
        //         this.movieClip.texture = tex;
        //         this.movieClip.node.active = true;

        //         this.scheduleOnce(()=>{

        //             this.node.width = this.movieClip.node.width;
        //             this.node.height = this.movieClip.node.height;

        //         },0.1)

        //     });
        // }

        // this.state = CharacterState.idle;

    }

    update (dt) 
    {
        // if(!SceneMap.instance)
        // {
        //     return;
        // }

        // super.update(dt);

        // if(SceneManager.instance.currentMapId == 0)
        // {
        //     return;
        // }

        // if(this.isPatrol)
        // {

        //     this.timer -= dt;

        //     if(this.timer <= 0)
        //     {
        //         this.timer = this.Range(1.5 , 4);

        //         this.targetPos.x = this.basePos.x + this.Range(-this.patrolRange , this.patrolRange);
        //         this.targetPos.y = this.basePos.y + this.Range(-this.patrolRange , this.patrolRange);
                
        //         this.patrol(this.targetPos.x,this.targetPos.y);

        //     }
        // }

    }

    public Range(num1:number,num2:number)
    {
        if(num2 > num1)
        {
            return Math.random() * (num2 - num1) + num1;
        }
        return Math.random() * (num1 - num2) + num2;
    }

    public patrol(targetX:number,targetY:number)
    {
        var startPoint:Point = MapRoadUtils.instance.getWorldPointByPixel(this.node.x,this.node.y);
        var targetPoint:Point = MapRoadUtils.instance.getWorldPointByPixel(targetX,targetY);

        var startNode:RoadNode = SceneMap.instance.getMapNodeByWorldPoint(startPoint.x,startPoint.y);
        var targetNode:RoadNode = SceneMap.instance.getMapNodeByWorldPoint(targetPoint.x,targetPoint.y);


        //var roadNodeArr:RoadNode[] = this._roadSeeker.seekPath(startNode,targetNode); //点击到障碍点不会行走
        var roadNodeArr:RoadNode[] = SceneMap.instance.roadSeeker.seekPath2(startNode,targetNode,250);  //点击到障碍点会行走到离障碍点最近的可走路点

        //cc.log():

        if(roadNodeArr.length > 0)
        {
            this.walkByRoad(roadNodeArr);
        }
    }

}
