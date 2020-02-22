// Learn TypeScript:
//  - https://docs.cocos.com/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - https://docs.cocos.com/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - https://docs.cocos.com/creator/manual/en/scripting/life-cycle-callbacks.html

const {ccclass, property} = cc._decorator;

@ccclass
export default class NewClass extends cc.Component {

    @property(cc.Label)
    label: cc.Label = null;

    @property(cc.Prefab) block: cc.Prefab = null

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

    }

    addBlock(data, id)
    {
        var block = cc.instantiate(this.block)
        this.node.addChild(block);

        var sj = block.getComponent("block");
        sj.setBlock(id)


        if ((data & 0x1) != 0)
        {
            block.color = cc.color(255,0,255) //block
        }
        else if ((data & 0x4) != 0)
        {
            block.color = cc.color(255,0,255) //WATER
        }
        else if ((data & 0x2) != 0)
        {
            block.color = cc.color(0,0,255) //WARK
        }
        else
        {
            block.color = cc.color(255,255,255)
        }
        // for(var i = 0 ; i <  blocks.length; i ++)
        // {

        // }
    }
    // update (dt) {}
}
