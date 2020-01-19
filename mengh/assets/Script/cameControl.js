
cc.Class({
    extends: cc.Component,

    properties: {
       
        tagNode : {
            default : null,
            type : cc.Node,
        },

    },

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {},

    //start () {},

    update (dt) {
        var spacePlace =  this.tagNode.convertToWorldSpaceAR(cc.p(0,0));
        var parentPlace = this.node.parent.convertToNodeSpaceAR(spacePlace);
        this.node.setPosition(parentPlace);
    },
});
