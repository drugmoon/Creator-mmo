cc.Class({
    extends: cc.Component,

    properties: {
        // foo: {
        //    default: null,
        //    url: cc.Texture2D,  // optional, default is typeof default
        //    serializable: true, // optional, default is true
        //    visible: true,      // optional, default is true
        //    displayName: 'Foo', // optional
        //    readonly: false,    // optional, default is false
        // },
        // ...
        ws:null,
        uid:null,
        ticks:null,
        maxtick:0,
        map: {
            default:null,
            type: cc.Node,
        }
    },

    sendmsg: function(msg){
       this.ws.send(msg);
    },

    // use this for initialization
    onLoad: function () {
         var map = this.node.getParent()
         this.ws = new WebSocket("ws://127.0.0.1:8001");
         this.ws.onopen = function (event) {
             console.log("Send Text WS was opened.");
             
             
         };
         this.ws.onmessage = function (event) {
         
            var reply = JSON.parse(event.data)//.data
            console.log("收到的信息为:"+JSON.stringify(reply))
            if(reply.type == "login_reply")
            {
                map.getComponent("map").uid = reply.uid
            }
            else if(reply.type == "tick")
            {
                map.getComponent("map").ticklist[reply.tick] = reply
                map.getComponent("map").maxtick = reply.tick
            }

         };
         this.ws.onerror = function (event) {
             console.log("Send Text fired an error");
         };
         this.ws.onclose = function (event) {
             console.log("WebSocket instance closed.");
         };
        
    },

    // called every frame, uncomment this function to activate update callback
    // update: function (dt) {

    // },
});
