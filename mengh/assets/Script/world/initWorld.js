
cc.Class({
    extends: cc.Component,

    properties: {
        worldBlockFab1  : {   //世界背景1
           default : null,
           type : cc.Prefab,
        },
        worldBlockFab2 : {      //世界背景2
            default : null,
            type : cc.Prefab,
        },
        houseFab : {            //用户城堡
            default : null,
            type : cc.Prefab,
        },
        stoneFab : {            //石头
            default : null,
            type : cc.Prefab,
        },
        foodFab : {             //食物
            default : null,
            type : cc.Prefab,
        },
        hoodFab : {         //木材
            default : null,
            type : cc.Prefab,
        },
        silverFab : {       //银矿
            default : null,
            type : cc.Prefab,
        },
        ironFab : {         //铁矿
            default : null,
            type : cc.Prefab,
        },
        mapNode : {
            default : null,
            type : cc.Node,
        },
        cameNode : {             //摄像机
            default : null,
            type : cc.Node,
        },
        speed : 1,
    },


    onLoad () {
        this.positionInit();
        //this.bgInit(400,20); //p1总数  p2是行数
        this.mapMove();
        this.reqBlockData();
    },

    //请求周围的block数据
    reqBlockData(){
        var self = this; 
        this.getMN();   
        var params = {
            sign : cc.vv.userMgr.sign,
            userid : cc.vv.userMgr.userid,
            m : this.m,
            n : this.n,
        };
        cc.vv.http.sendRequest("/getBlockData",params,function(data){
            if(data.code == 1){
                cc.log("errmsg:",data.msg);
                return;
            }
            self.updateBlocks(data.rows);
        },cc.vv.url.cityServer);
    },

    //处理请求返回数据
    updateBlocks(rows){

        var wNode = this.node.getChildByName("wall");
        //清除之前的孩子节点
        wNode.removeAllChildren();
        for(var  i = 0; i< rows.length; i++){
            var  block  = rows[i];
            //得到他的x,y的坐标
            this.getXYformLocal(block.x,block.y);
            //生成草地
            this.produceGreen(block.x,block.y);

            switch(block.type){
                case "player":
                            cc.log("this is a player!");
                            var hNode  = cc.instantiate(this.houseFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            var nameNode = hNode.getChildByName("info").getChildByName("player");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly + 20;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            nameNode.getComponent(cc.Label).string = block.name;
                            break;
                case "food":
                            cc.log("this is a food!");
                            var hNode = cc.instantiate(this.foodFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            break;
                case "hood":
                            cc.log("this is a hood!");
                            var hNode = cc.instantiate(this.hoodFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            break;
                case "stone":
                            cc.log("this is a stone!");
                            var hNode = cc.instantiate(this.stoneFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            break;
                case "iron":
                            cc.log("this is a iron!");
                            var hNode = cc.instantiate(this.ironFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            break;
                case "silver":
                            cc.log("this is a silver!");
                            var hNode = cc.instantiate(this.silverFab);
                            var lvNode = hNode.getChildByName("info").getChildByName("lv").getChildByName("lvStr");
                            wNode.addChild(hNode);
                            hNode.x = this.lx;
                            hNode.y = this.ly;
                            lvNode.getComponent(cc.Label).string = block.lv;
                            break;
                default  :
                            cc.log("this is default!");                          
            }

        }
    },

    //生成一片草地
    produceGreen(x,y){
        var  sum = parseInt(x) + parseInt(y);
        var  tNode =  null;
        if((sum)%2 == 0){
            tNode = cc.instantiate(this.worldBlockFab1);
        }else{
            tNode = cc.instantiate(this.worldBlockFab2);
        }
        this.mapNode.addChild(tNode);

            tNode.x  = this.lx;
            tNode.y  = this.ly;
    },

    //得到本地坐标
    getXYformLocal(x,y){
        var cx = parseInt(x);
        var cy = parseInt(y);
        this.lx = 126*(cy - cx);
        this.ly = 76*(cx + cy);
    },

    //得到中心点位置
    getMN(){
        var  cx = parseInt(this.cameNode.x);
        var  cy = parseInt(this.cameNode.y);
        cc.log("cx:",cx);
        cc.log("cy:",cy);
        var  x = Math.round(cy/152 - cx/252);
        var  y = Math.round(cx/252 + cy/152);
        this.m = x;
        this.n = y;
        cc.log("m:",this.m);
        cc.log("n:",this.n);
    },

    //初始化摄像机
    positionInit(){
        var x  = parseInt(cc.vv.userMgr.x);
        var y  = parseInt(cc.vv.userMgr.y);
        cc.log("x1:",x);
        cc.log("y1:",y);
        var cx = this.cameNode.x;
        var cy = this.cameNode.y;
        cc.log("cx1:",cx);
        cc.log("cy1:",cy);
        this.sx = 126*(y - x);  //原x坐标
        this.sy = 76*(x + y);   //原y坐标
        this.cameNode.x = this.sx;
        this.cameNode.y = this.sy;
        cc.log("this.cameraNode.x:",this.cameNode.x);
        cc.log("this.cameraNode.y:",this.cameNode.y);
    },
    
    //移动地图
    mapMove(){

        this.node.on(cc.Node.EventType.TOUCH_START, function(e){

         }, this);
         this.node.on(cc.Node.EventType.TOUCH_MOVE, function(e){

             var lx = e.getDeltaX();
             var ly = e.getDeltaY();
             var x = this.cameNode.x;
             var y = this.cameNode.y;
             //if(x - lx * this.speed<1140 && x -lx * this.speed>-1140){
                 this.cameNode.x = x - lx * this.speed;
            // }
             //if(y - ly * this.speed<384 && y - ly * this.speed>-384){
                 this.cameNode.y = y - ly * this.speed;
             //}
             //刷新数据
            var tx = parseInt(x - this.sx);
            var ty = parseInt(y - this.sy);
            var distance = parseInt(Math.sqrt(tx*tx + ty*ty)); 
            if(distance > 600){   //移动距离触发刷新事件
                this.sx = x;
                this.sy = y;
                this.reqBlockData();
            }
 
         }, this);
         this.node.on(cc.Node.EventType.TOUCH_END, function(e){
             //cc.log("touch end");
         }, this);
         this.node.on(cc.Node.EventType.TOUCH_CANCEL, function(e){
             //cc.log("touch cancel");
         }, this);
    },

    //拼接背景
    bgInit(num,block){
        var  m = parseInt(this.cameNode.x);
        var  n = parseInt(this.cameNode.y)-1000;
        var x = m;
        var y = n;
        var tNode = null;
        var round = 0;
        var remainder = 0;
        var even = 0;
        for(var i=0; i<num; i++){
            if((i+even)%2 == 0){
                tNode =  cc.instantiate(this.worldBlockFab1);
            }else{
                tNode =  cc.instantiate(this.worldBlockFab2);
            }
           this.mapNode.addChild(tNode);

                    tNode.x = x;
                    tNode.y = y;

            remainder  = (i+1) % block;
            if(remainder==0){
                even = even + 1;
                round = parseInt((i+1)/block);
                x = m - 126 * round;
                y = n + 76 * round;
            } else{
                x =  x + 126;
                y = y + 76;
            }

        }
    },


    start () {

    },

    // update (dt) {},
});
