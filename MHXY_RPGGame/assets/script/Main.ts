// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import SceneMap from "./SceneMap";
import MapData from "./map/base/MapData";
import { MapLoadModel } from "./map/base/MapLoadModel";
import SceneManager from "./SceneManager";

const {ccclass, property} = cc._decorator;

@ccclass
export default class Main extends cc.Component {



    // LIFE-CYCLE CALLBACKS:

    onLoad () 
    {
        cc.director.getCollisionManager().enabled= true;
    }

    start () {

        cc.debug.setDisplayStats(false);

        SceneManager.instance.loadMap(10001,1,MapLoadModel.slices);

       // SceneManager.instance.loadMap(9999,1,MapLoadModel.single);
    }

    // update (dt) {}
}
