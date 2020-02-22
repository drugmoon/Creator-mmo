 import { FishState, FishType, MAX_SIZE,TILE_WIDTH, TILE_HEIGHT } from './ComDefine'
import MapManager  from '../MapManager'
import BinManager from '../BinManager'
const {ccclass, property} = cc._decorator;
 
@ccclass
export default class ByteArray extends cc.Component {

    @property(cc.Node) map: cc.Node = null;
    @property(cc.Prefab) blocklist: cc.Prefab = null
    @property(cc.Node) mainCamera: cc.Node = null
    
    speed = 1
   
    start () {

        let message = {name: "hello", pwd: "pwd"};

        this.map.on(cc.Node.EventType.TOUCH_MOVE, function(e){
            //cc.log("touch move");
            var lx = e.getDeltaX();
            var ly = e.getDeltaY();
            var x = this.mainCamera.x;
            var y = this.mainCamera.y;
         //   if(x - lx * this.speed<70000 && x -lx * this.speed>-7000){
                this.mainCamera.x = x - lx * this.speed;
          //  }
          //  if(y - ly * this.speed<50000 && y - ly * this.speed>-50000){
                this.mainCamera.y = y - ly * this.speed;
        //    }
            

        }, this);

       




        BinManager.getInstance().loadBiz(1,2)
    }
 
    setScaleAdd()
    {
        this.map.scale += 0.1
    }
    setScaleSub()
    {
        this.map.scale -= 0.1
    }
 
    loadMap()
    {
        //清理
        this.map.removeAllChildren()

        let loadUrl = cc.find("root/tool/loadUrl").getComponent(cc.EditBox);

        cc.log("loadUrl " + loadUrl.string)

        let url = "resources/map/"+  loadUrl.string + ".mapo"

        MapManager.getInstance().initMap(url,()=>{
            cc.log("initMap")

            //设置 map 大小
            this.map.width = MapManager.getInstance().mlogic_width * TILE_WIDTH
            this.map.height = MapManager.getInstance().mlogic_height * TILE_HEIGHT

            for (var i = 0; i < MapManager.getInstance().getBlocklength(); i++)
            {
                if (i % (MapManager.getInstance().mlogic_width) == 0 )
                {
                    var blocklist = cc.instantiate(this.blocklist)
                    var sj = blocklist.getComponent("block_list");
                    this.map.addChild(blocklist);
                }
                sj.addBlock(MapManager.getInstance().mBlockData[i], i);
            }
        })
    }
    // update (dt) {}
}
