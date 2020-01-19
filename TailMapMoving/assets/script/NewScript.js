
cc.Class({
    extends: cc.Component,

    properties: {
        okBtn: cc.Button
    },

    onLoad: function () {
        
    },

    start () {
        //2.1.1的一个bug，本点击实现若先注册，会被其他点击事件覆盖无效，即便加上了BlockInputEvents组件。UI脚本放在层级管理器的下边。据说2.1.2会修复
        this.okBtn.node.on('click', this.callback, this);
    },

    callback: function (button) {
        this.node.active = false;
    }

});
