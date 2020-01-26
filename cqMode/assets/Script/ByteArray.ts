 

const {ccclass, property} = cc._decorator;

@ccclass
export default class ByteArray extends cc.Component {
    private static instance: ByteArray = null;
    public static getInstance(): ByteArray {
        this.instance = this.instance || new ByteArray()
        return this.instance
    }

    private m_size:number = 0;
	private m_pos:number = 0;
    private m_buf = null;
    private dv:DataView = null;
    private data = null;
    private path = "123"
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
    loadData(url, callback: Function)
    {
        var path = cc.url.raw("resources/map/v102.mapo");
        var self = this
        cc.loader.load({ url: path, type: "binary", }, function (err, data) {
            self.data = data
            self.setData(callback)
        });
    }
    setData(callback: Function)
    {
        cc.log("binaryData " + this.data)
        let buffer = new ArrayBuffer(this.data.length);
        this.dv = new DataView(buffer);
        for (var i = 0; i < this.data.length; ++i) {
        
            this.dv.setUint8(i, this.data[i])
        }
        callback()
    }
    start () {

    }

    // update (dt) {}
}
