

let PF = require("pathfinding");

cc.Class({
	extends: cc.Component,
	editor: {
		requireComponent: cc.TiledMap
	},

	properties: {
		mainCamera: {
			default: null,
			type: cc.Node
		},
		player: {
			default: null,
			type: cc.Node
		},

		winLayout: {
			default: null,
			type: cc.Node
		},

		floorLayerName: {
			default: 'floor'
		},

		barrierLayerName: {
			default: 'barrier'
		},

		objectGroupName: {
			default: 'players'
		},
		successObjectName: {
			default:'SuccessPoint'
		},

		_barrierGrid: {
			default:null
		},

		_pathFind: {
			default:null
		},

		_tileSize: {
			default:null
		},

		_mapSize: {
			default:null
		},

		_isMoving: {
			default: false
		}
	},

    // use this for initialization
    onLoad: function () {
		this._resetMapPos();
    },

    start: function(err) {
    	if (err) return;
		
    	this._tiledMap = this.node.getComponent('cc.TiledMap');
    	this._layerFloor = this._tiledMap.getLayer(this.floorLayerName);
    	this._layerBarrier = this._tiledMap.getLayer(this.barrierLayerName);
    	this._mapSize = this._tiledMap.getMapSize();
		this._tileSize = this._tiledMap.getTileSize();
		
		//设置障碍网格和寻路
    	// let blockArr = new Array();
    	// for (let y=0; y<this._mapSize.height; y++) {
    	// 	let xArr = new Array();
    	// 	for (let x=0; x<this._mapSize.width; x++) {
    	// 		let isBlock = this._layerBarrier.getTileGIDAt(cc.v2(x, y))?1:0;
    	// 		xArr[x] = isBlock;
    	// 	}
    	// 	blockArr[y] = xArr;
    	// }
    	// this._barrierGrid = new PF.Grid(blockArr);
    	// this._pathFind = new PF.AStarFinder();

		// //设置迷宫终点位置
    	// let objectGroup = this._tiledMap.getObjectGroup(this.objectGroupName);
    	// let endObj = objectGroup.getObject(this.successObjectName);
    	// let endPos = cc.v2(endObj.x, endObj.y);
		// this._endTile = this._pixelToTile(endPos);

		//注册事件
		this.node.on(cc.Node.EventType.TOUCH_START, (event) => {
			let touchPos = event.touch.getLocation();
			let targetPos = cc.v2();
			// this.mainCamera.getComponent(cc.Camera).getCameraToWorldPoint(touchPos, targetPos)
    		// let tilePos = this._pixelToTile(targetPos);
			// this._moveToTile(tilePos);
			
			var x = this.mainCamera.node.x;
            var y = this.mainCamera.node.y;
            if(x - lx * this.speed<4800 && x -lx * this.speed>0){

                this.mainCamera.x = x - lx * this.speed;
            }
            if(y - ly * this.speed<2240 && y - ly * this.speed>0){
                this.mainCamera.y = y - ly * this.speed;
            }
		});
	},
	update(dt) {
        // 更新摄像机         
       // this.updateNearCamera(dt);
        
       
    },
	updateNearCamera(dt) {
        this.mainCamera.x += dt* 300;
    },
    onDestroy () {
    },

    _resetMapPos: function() {
    	this.node.setPosition(cc.visibleRect.bottomLeft);
	},
	
    _pixelToTile: function(posInPixel) {
    	let mapSize = this.node.getContentSize();
    	let x = Math.floor(posInPixel.x / this._tileSize.width);
    	let y = (mapSize.height - posInPixel.y) / this._tileSize.height;
    	y = Number.isInteger(y)?--y:Math.floor(y);//y取整，为整时，取上方tile。若入参为锚点，需先假设锚点y不为1
		//if(posInPixel.y%this._tileSize.height==0) y--;
    	return cc.v2(x, y);
    },

    _moveToTile: function(newTile) {
		if (this._isMoving) return;
    	if (newTile.x < 0 || newTile.x >= this._tileSize.width) return;
    	if (newTile.y < 0 || newTile.y >= this._tileSize.height) return;

    	if (this._layerBarrier.getTileGIDAt(newTile)) {
    		cc.log('This way is blocked!');
    		return false;
    	}

        let barrierGridTmp = this._barrierGrid.clone();

        let nowPlayerPos = this._pixelToTile(this.player.getPosition());

        let path = this._pathFind.findPath(nowPlayerPos.x, nowPlayerPos.y, newTile.x, newTile.y, barrierGridTmp);
		//let newPath = PF.Util.smoothenPath(barrierGridTmp, path);

		let tween = cc.tween(this.player);
        path.forEach((v, k)=>{
			if(k===0) return;
            tween.to(0.1, {position: this._layerFloor.getPositionAt(cc.v2(v[0], v[1]))})
		});
		tween.call(() => {this._onMovingEnd()});
		tween.start();
		this._isMoving = true;

	},
	
	_onMovingEnd: function() {
		this._isMoving=false;
		// check the player is success or not
        if (this._pixelToTile(this.player.getPosition()).equals(this._endTile)) {
        	this.winLayout.active = true;
		};

        this._tryMoveCamera();
		
	},

    _tryMoveCamera: function() {
        // get necessary data
        let mapContentSize = this.node.getContentSize();
        let playerPos = this.player.getPosition();

		//计算坐标，使人物在摄像机中心
        let disX = playerPos.x - this.mainCamera.x;
        let disY = playerPos.y - this.mainCamera.y;
		let newPos = cc.v2();
		if (true) {
			newPos.y = this.mainCamera.y + (disY-cc.visibleRect.height/2);
		}
		if (true) {
			newPos.x = this.mainCamera.x + (disX-cc.visibleRect.width/2);
		}
		
		//坐标修正，不超过map边界
		if(newPos.y<0){
			newPos.y = 0;
		}else{
			if(newPos.y+cc.visibleRect.height > this.node.height){
				newPos.y = this.node.height - cc.visibleRect.height;
			}
		}

		if(newPos.x<0){
			newPos.x = 0;
		}else{
			if(newPos.x+cc.visibleRect.width > this.node.width){
				newPos.x = this.node.width - cc.visibleRect.width;
			}
		}
		
		//移动摄像机
		if(playerPos.equals(newPos)) return;
		cc.tween(this.mainCamera).to(0.1, {
			position: newPos
		}).start();
		

    }
});
