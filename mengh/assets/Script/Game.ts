
const { ccclass, property } = cc._decorator;

@ccclass
export default class Game extends cc.Component {
 
    // 近景摄像机
    @property(cc.Camera) mapCamera: cc.Camera = null;
 
    @property(cc.Label) pos = null;
 
    @property(cc.Node) bg = null;

    mapSpeed = 300;
    speed = 1;
    start() {
        
    }

    onLoad() {

        this.node.on(cc.Node.EventType.TOUCH_MOVE, function(e){
            //cc.log("touch move");
            var lx = e.getDeltaX();
            var ly = e.getDeltaY();

          //  this.pos.string = lx + " , " + ly
            var x = this.mapCamera.node.x;
            var y = this.mapCamera.node.y;
            if(x - lx * this.speed<4800 && x -lx * this.speed>0){

                this.mapCamera.node.x = x - lx * this.speed;
            }
            if(y - ly * this.speed<2240 && y - ly * this.speed>0){
                this.mapCamera.node.y = y - ly * this.speed;
            }
          
            
        }, this);


        // this.bg.on(cc.Node.EventType.TOUCH_START, function(event){
        //     cc.log("touch move");
        //     let touchPos = event.touch.getLocation();
		// 	let targetPos = cc.v2();
		// 	this.mapCamera.getComponent(cc.Camera).getCameraToWorldPoint(touchPos, targetPos)
    	//  	//let tilePos = this._pixelToTile(targetPos);

        //     cc.log("pos " + targetPos.x)
        //     cc.log("pos " + targetPos.y)

        // }, this);

    }

    _tilePosition(pixelPosition) {
        // let mapSize = this.node.getContentSize();
        // let tileSize = this._tiledMap.getTileSize();
        // let x = Math.floor(pixelPosition.x / tileSize.width);
        // let y = Math.floor((mapSize.height - pixelPosition.y) / tileSize.height);

        return cc.v2(x, y);
    }

    update(dt) {
        // 更新摄像机         
        this.updateNearCamera(dt);
        
       
    }
 

    updateNearCamera(dt: number) {
        this.mapCamera.node.x += dt*this.mapSpeed;
 
 

    }
    
}
