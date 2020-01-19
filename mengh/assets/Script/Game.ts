
const { ccclass, property } = cc._decorator;

@ccclass
export default class Game extends cc.Component {
 
    // 近景摄像机
    @property(cc.Camera) mapCamera: cc.Camera = null;
 
    @property(cc.Label) pos = null;
 
    mapSpeed = 300;
    speed = 1;
    start() {
        
    }

    onLoad() {


        this.node.on(cc.Node.EventType.TOUCH_MOVE, function(e){
            cc.log("touch move");
            var lx = e.getDeltaX();
            var ly = e.getDeltaY();
            cc.log("lx " + lx)
            cc.log("ly " + ly)
            this.pos.string = lx + " , " + ly
            var x = this.mapCamera.node.x;
            var y = this.mapCamera.node.y;
            if(x - lx * this.speed<2400 && x -lx * this.speed>-2400){

                this.mapCamera.node.x = x - lx * this.speed;
            }
            if(y - ly * this.speed<1120 && y - ly * this.speed>-1120){
                this.mapCamera.node.y = y - ly * this.speed;
            }
          

        }, this);

    }
    update(dt) {
        // 更新摄像机         
        
         //this.updateNearCamera(dt);
        
       
    }
 

    updateNearCamera(dt: number) {
        this.mapCamera.node.x += dt*this.mapSpeed;
 
 

    }
    
}
