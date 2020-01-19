cc.Class({
    extends: cc.Component,

    properties: {
       speed : 1,
 
        cameNode : {             //摄像机
            default : null,
            type : cc.Node,
        },
    },

    // use this for initialization
    onLoad: function () {
        this.node.on(cc.Node.EventType.TOUCH_START, function(e){
           //清除之前的小图标
            // var topshow = this.topshow;
            // if(topshow.childrenCount>0){
            //     var tgchildArry = topshow.children;
            //     for(var i = 0; i < tgchildArry.length; i++){
            //     tgchildArry[i].destroy();
            //     }
            // }
        
        }, this);
        this.node.on(cc.Node.EventType.TOUCH_MOVE, function(e){
            cc.log("touch move");
            // var lx = e.getDeltaX();
            // var ly = e.getDeltaY();
            // var x = this.cameNode.x;
            // var y = this.cameNode.y;
            // if(x - lx * this.speed<2400 && x -lx * this.speed>-2400){
            //     this.cameNode.x = x - lx * this.speed;
            // }
            // if(y - ly * this.speed<1120 && y - ly * this.speed>-1120){
            //     this.cameNode.y = y - ly * this.speed;
            // }
            

        }, this);
        this.node.on(cc.Node.EventType.TOUCH_END, function(e){
            //cc.log("touch end");
        }, this);
        this.node.on(cc.Node.EventType.TOUCH_CANCEL, function(e){
            //cc.log("touch cancel");
        }, this);
    },

    // called every frame, uncomment this function to activate update callback
    // update: function (dt) {

    // },
});
