 
 
//import { resolve } from 'path'
 
const {ccclass, property} = cc._decorator;
let MAX_SIZE = 1024*24;
@ccclass
export default class ByteArray extends cc.Component {

    @property(cc.Node) map: cc.Node = null;
 
  
    @property(cc.Prefab) blocklist: cc.Prefab = null
 

    @property(cc.Node) mainCamera: cc.Node = null
    

    // LIFE-CYCLE CALLBACKS:
    data = null;
    dv:DataView = null;
    // onLoad () {}
    array = []
    path = "123"
    m_pos = 0
    speed = 1
    down = false
    start () {

     
       let message = {name: "hello", pwd: "pwd"};
   
  
        var path = cc.url.raw("resources/map/v102.mapo");
        this.build(path)

        var self = this;

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

    }
 
 
    readInt()
    {
        var buf = this.dv.getInt32(this.m_pos, true)
        this.m_pos += 4;
        return buf
    }
    readShort()
    {
        var buf = this.dv.getInt16(this.m_pos, true)
        this.m_pos += 2;
        return buf
    }
    readUShort()
    {
        var buf = this.dv.getUint16(this.m_pos, true)
        this.m_pos += 2;
        return buf
    }
    readByte()
    {
        var buf = this.dv.getInt8(this.m_pos)
        this.m_pos += 1;
        return buf
    }
    readUByte()
    {
        var buf = this.dv.getUint8(this.m_pos)
        this.m_pos += 1;
        return buf
    }
    setData()
    {
        cc.log("binaryData " + this.data)

        let buffer = new ArrayBuffer(this.data.length);
 
        this.dv = new DataView(buffer);
        for (var i = 0; i < this.data.length; ++i) {
        
            this.dv.setUint8(i, this.data[i])
        }

  
     
        var mVersion = this.readUShort();

        console.log("mVersion " + mVersion)

        if(true)
        {
            var mGroundWidth=this.readShort();
            var mGroundHeight=this.readShort();
            var logic_width=this.readShort();
            var logic_height=this.readShort();
            var mGroundDir=this.readByte();
            var size=this.readInt();

            cc.log("mGroundWidth " + mGroundWidth)
            cc.log("mGroundHeight " + mGroundHeight)
            cc.log("logic_width " + logic_width)
            cc.log("logic_height " + logic_height)
            cc.log("mGroundDir " + mGroundDir)
            cc.log("size " + size)
            var i=0;
            var badindex = 0;
            var ind = 0;
            var b;
            var l;
            var mBlockData = []
            while(ind<size)
            {
                b=this.readUByte();
                l=this.readUByte();
                for (i = 0; i < l; i++)
                {
                    mBlockData[badindex++] = b & 0xFF;
                    //log("readUByte %d", b );
                    //log("readUByte & 0xFF %d", b & 0xFF);
                }
                ind = ind + 2;  //这里加2是因为连续获取了两个readUByte  字节。
            }

            var pstr = "";
            var len = 0;
            // for (i = 0; i < logic_width*logic_height; i++)
            // {
 
            //    // cc.log("mBlockData " +  mBlockData[i]);
            // }
        }

        for (var i = 0; i < logic_width*logic_height; i++)
        {

            if (i % (logic_width) == 0 )//&& i != 0
            {
                var blocklist = cc.instantiate(this.blocklist)
                var sj = blocklist.getComponent("block_list");
                this.map.addChild(blocklist);
            }
            sj.addBlock(mBlockData[i], i);
        }

        // mBlockData=new unsigned char[logic_width*logic_height];
		// memset(mBlockData,0,logic_width*logic_height);
		// mGhostData=new unsigned char[logic_width*logic_height];
		// memset(mGhostData,0,logic_width*logic_height);

		// int i=0;
		// int badindex = 0;
		// int ind = 0;
		// unsigned char b;
		// unsigned char l;
		// while(ind<size)
		// {
		// 	b=ba->readUByte();
		// 	l=ba->readUByte();
		// 	for (i = 0; i < l; i++)
		// 	{
		// 		mBlockData[badindex++] = b & 0xFF;
		// 		//log("readUByte %d", b );
		// 		//log("readUByte & 0xFF %d", b & 0xFF);
		// 	}
		// 	ind = ind + 2;  //这里加2是因为连续获取了两个readUByte  字节。
		// }
    }
    setScaleAdd()
    {
        this.map.scale += 0.1
    }
    setScaleSub()
    {
        this.map.scale -= 0.1
    }
	// static ByteArray* build(int size=MAX_SIZE);
	// static ByteArray* buildC(char* buf,size_t size,bool dec=false);
    build(path:string, size = MAX_SIZE)
    {
        var self = this
        cc.loader.load({ url: path, type: "binary", }, function (err, data) {
            // if (data instanceof ArrayBuffer){
            //     data = new Uint8Array(data);
            // }
            console.log("err ===", err);
            console.log("data ===", data.length);      
            cc.log("path " + self.path)
          //  cc.log("data " + data)

            self.data = data

            self.setData()
        });
    }
    // update (dt) {}
}
