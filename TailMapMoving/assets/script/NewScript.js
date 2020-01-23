
cc.Class({
    extends: cc.Component,

    properties: {
        okBtn: cc.Button,
   
 
 
 
    },

    onLoad: function () {
        
    },

    start () {
        //2.1.1的一个bug，本点击实现若先注册，会被其他点击事件覆盖无效，即便加上了BlockInputEvents组件。UI脚本放在层级管理器的下边。据说2.1.2会修复
        //this.okBtn.node.on('click', this.callback, this);

        //this.anim.play("1")


		//注册事件
 
        let self = this
      //  var equips = cc.find("equips")
        // self.node.on('mouseup', function(event){
            
        //     console.log("event.getButton() is " + event.getButton())
        //     if(event.getButton() == cc.Event.EventMouse.BUTTON_RIGHT){
        //     // var x = event.getLocation().x - self.map.x
        //     // var y = event.getLocation().y - self.map.y
        //     // var dot = self.map.getChildByName("pass_dot")
        //     // dot.stopAllActions()
        //     // dot.x = x
        //     // dot.y = y
        //     // var action1 = cc.fadeIn(0.1);
        //     // var action2 = cc.scaleTo(0.2, 1, 1);
        //     // var action3 = cc.scaleTo(0.2, 0.4, 0.4);
        //     // var action4 = cc.scaleTo(0.2, 0.7, 0.7);
        //     // var action5 = cc.fadeOut(1);

        
        //     // var seq1 = cc.sequence(action1, action2, action3, action4, action5);

        //     // dot.runAction(seq1)

    
        //     // self.socket.getComponent("socket_l").sendmsg(JSON.stringify(json1))
        //     // cc.find("herodesc").active = false
        //     }


        // })
    },

    callback: function (button) {
        this.node.active = false;
    }

});
