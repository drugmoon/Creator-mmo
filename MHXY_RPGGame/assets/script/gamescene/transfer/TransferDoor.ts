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
import SpawnPoint from "./SpawnPoint";
import Player from "../player/Player";
import SceneManager from "../../SceneManager";
import { MapLoadModel } from "../../map/base/MapLoadModel";

const {ccclass, property} = cc._decorator;

/**
 * 传送门
 */
@ccclass
export default class TransferDoor extends cc.Component {

    /**
     * 在本地图，自己的传送点Id
     */
    @property(cc.Integer)
    public transferId: number = 0;

    /**
     * 传送到目标地图Id
     */
    @property(cc.Integer)
    public targetMapId: number = 0;

    /**
     * 传送到目标地图的出生点Id
     */

    @property(cc.Integer)
    public targetMapSpawnId: number = 0;

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

        //this.node.opacity = 0;
    }

    // update (dt) {}

    public toString()
    {
        return this.transferId + " : " + this.targetMapId + "," + this.targetMapSpawnId;
    }

    /**
     * 进入传送门
     * @param callback 
     */
    public onTriggerEnter(player:Player)
    {
        SceneManager.instance.loadMap(this.targetMapId,this.targetMapSpawnId,MapLoadModel.slices);
    }

    /**
     * 从传送们出来
     * @param callback 
     */
    public onTriggerExit(player:Player)
    {
        
    }
}
