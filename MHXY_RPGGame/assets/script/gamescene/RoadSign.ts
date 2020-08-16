// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import MovieClip from "../map/character/MovieClip";



const {ccclass, property} = cc._decorator;

/**
 * 路标
 */
@ccclass
export default class RoadSign extends cc.Component {

    @property(MovieClip)
    signMc: MovieClip = null;


    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

        this.signMc.node.on("complete",()=>{
            this.node.active = false;
        },this);
    }

    // update (dt) {}

    public play()
    {
        this.node.active = true;
        this.signMc.reset();
        this.signMc.play();
    }
}
