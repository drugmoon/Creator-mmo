  
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
    public getBlocklength(){
        return this.mlogic_width * this.mlogic_height;
    }
    public initMap(callback: Function){

        ByteArray.getInstance().loadData("", () => {

            this.initData()
            callback()
        })
    }

    initData(){
        var mVersion = ByteArray.getInstance().readUShort();
        console.log("mVersion " + mVersion)
        if(mVersion == 4)
        {
            this.mGroundWidth = ByteArray.getInstance().readShort();
            this.mGroundHeight = ByteArray.getInstance().readShort();
            this.mlogic_width = ByteArray.getInstance().readShort();
            this.mlogic_height = ByteArray.getInstance().readShort();
            this.mGroundDir = ByteArray.getInstance().readByte();
            var size = ByteArray.getInstance().readInt();

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
                b = ByteArray.getInstance().readUByte();
                l = ByteArray.getInstance().readUByte();
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
