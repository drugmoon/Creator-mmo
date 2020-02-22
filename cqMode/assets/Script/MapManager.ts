  
import ByteArray  from './ByteArray'

const {ccclass, property} = cc._decorator;

@ccclass
export default class MapManager extends cc.Component {
    private static instance: MapManager = null;
    public static getInstance(): MapManager {
        this.instance = this.instance || new MapManager()
        return this.instance
    }

    public mBlockData = [];
    public mGroundWidth = 0;
    public mGroundHeight = 0;
    public mlogic_width = 0;
    public mlogic_height = 0;
    public mGroundDir = 0
    private _ByteArray
    public getBlocklength(){
        return this.mlogic_width * this.mlogic_height;
    }
      
    onDestroy(){

        this._ByteArray.destroy()
    }
    public initMap(url ,callback: Function){
        this._ByteArray = new ByteArray()
        //"resources/map/v102.mapo"
        this._ByteArray.loadData(url, () => {

            this.initData()
            callback()
        })
    }

    initData(){
        var mVersion = this._ByteArray.readUShort();
        console.log("mVersion " + mVersion)
        if(mVersion == 4)
        {
            this.mGroundWidth = this._ByteArray.readShort();
            this.mGroundHeight = this._ByteArray.readShort();
            this.mlogic_width = this._ByteArray.readShort();
            this.mlogic_height = this._ByteArray.readShort();
            this.mGroundDir = this._ByteArray.readByte();
            var size = this._ByteArray.readInt();

            cc.log("mGroundWidth " + this.mGroundWidth)
            cc.log("mGroundHeight " + this.mGroundHeight)
            cc.log("logic_width " + this.mlogic_width)
            cc.log("logic_height " + this.mlogic_height)
            cc.log("mGroundDir " + this.mGroundDir)
            cc.log("size " + size)
            var i=0;
            var badindex = 0;
            var ind = 0;
            var b;
            var l;
            while(ind<size)
            {
                b = this._ByteArray.readUByte();
                l = this._ByteArray.readUByte();
                for (i = 0; i < l; i++)
                {
                    this.mBlockData[badindex++] = b & 0xFF;
                }
                ind = ind + 2;  //这里加2是因为连续获取了两个readUByte  字节。
            }
        }
    }

    start () {

    }

    // update (dt) {}
}
