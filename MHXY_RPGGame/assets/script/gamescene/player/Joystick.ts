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
export default class Joystick extends cc.Component {

    @property(cc.Node)
    private cursor: cc.Node = null;

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {
        //this.node.scale = 2.5;
    }

    public cursorTo(dir:cc.Vec2)
    {
        /*var angle:number = Math.atan2(dir.y,dir.x);
        this.cursor.x = 20 * Math.cos(angle);
        this.cursor.y = 20 * Math.sin(angle);*/
        this.cursor.position = dir.mul(20);
    }

    public show()
    {
        this.node.active = true;
    }

    public hidden()
    {
        this.node.active = false;
    }

    // update (dt) {}
}
