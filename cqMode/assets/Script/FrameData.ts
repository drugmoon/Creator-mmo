 
const {ccclass, property} = cc._decorator;

class FrameInfo extends cc.Component {
    public id;
	public frame;
	public rotated;
	public offset;
	public sourceSize;
}

@ccclass
class FrameData extends cc.Component {
    public mFileID = 0;
	public mDirCount = 0;
	public mFrameCount = 0;
    public mFrameData:any[][] = [];
    private my2DArray : Array<Array<any>> = new Array<Array<any>>();

    public FileID(){return this.mFileID;}
	public DirCount(){return this.mDirCount;}
    public FrameCount(){return this.mFrameCount;}
    
    GetInfo(dir,index)
    {
		if(dir >= 0 && dir < this.mDirCount && index >= 0 && index < this.mFrameCount)
		{
			if(this.mFrameData[dir][index])
			{
				return this.mFrameData[dir][index];
			}
		}
		return null;
    }
    start () {

    }

    // update (dt) {}
}


 
export {FrameData, FrameInfo}