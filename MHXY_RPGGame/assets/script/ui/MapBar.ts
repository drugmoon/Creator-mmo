// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import SceneManager from "../SceneManager";
import { MapLoadModel } from "../map/base/MapLoadModel";

const {ccclass, property} = cc._decorator;

@ccclass
export default class MapBar extends cc.Component {

    @property(cc.Node)
    mapItems: cc.Node[] = [];


    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

        for(let i = 0 ; i < this.mapItems.length ; i++)
        {
            this.mapItems[i].on(cc.Node.EventType.TOUCH_START,(event:cc.Event.EventTouch)=>
            {
                SceneManager.instance.loadMap(10000 + (i + 1),1,MapLoadModel.slices);
            });
        }

    }

    // update (dt) {}
}
