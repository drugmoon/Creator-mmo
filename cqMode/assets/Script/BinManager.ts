// Learn TypeScript:
//  - https://docs.cocos.com/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - https://docs.cocos.com/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - https://docs.cocos.com/creator/manual/en/scripting/life-cycle-callbacks.html

const {ccclass, property} = cc._decorator;

@ccclass
export default class BinManager extends cc.Component {
    private static instance: BinManager = null;
    public static getInstance(): BinManager {
        this.instance = this.instance || new BinManager()
        return this.instance
    }




    start () {

    }

    // update (dt) {}
}
